<a name="arangoimp"></a>
# Arangoimp

This manual describes the ArangoDB importer _arangoimp_, which can be used for
bulk imports.

The most convenient method to import a lot of data into ArangoDB is to use the
`arangoimp` command-line tool. It allows you to import data records from a file
into an existing database collection.

It is possible to import document keys with the documents using the `_key`
attribute. When importing into an edge collection, it is mandatory that all
imported documents have the `_from` and `_to` attributes, and that they contain
valid references.


Let's assume for the following examples you want to import user records into an 
existing collection named "users" on the server.

<a name="importing_data_into_an_arangodb_database"></a>
## Importing Data into an ArangoDB Database

<a name="importing_json-encoded_data"></a>
### Importing JSON-encoded Data

Let's further assume the import at hand is encoded in JSON. We'll be using these
example user records to import:

@verbinclude arangoimp-data-json

To import these records, all you need to do is to put them into a file (with one
line for each record to import) and run the following command:

    unix> arangoimp --file "data.json" --type json --collection "users"

This will transfer the data to the server, import the records, and print a
status summary. To show the intermediate progress during the import process, the
option `--progress` can be added. This option will show the percentage of the
input file that has been sent to the server. This will only be useful for big
import files.

    unix> arangoimp --file "data.json" --type json --collection "users" --progress true

By default, the endpoint `tcp://127.0.0.1:8529` will be used.  If you want to
specify a different endpoint, you can use the `--server.endpoint` option. You
probably want to specify a database user and password as well.  You can do so by
using the options `--server.username` and `--server.password`.  If you do not
specify a password, you will be prompted for one.

    unix> arangoimp --server.endpoint tcp://127.0.0.1:8529 --server.username root --file "data.json" --type json --collection "users"

Note that the collection (`users` in this case) must already exist or the import
will fail. If you want to create a new collection with the import data, you need
to specify the `--create-collection` option. Note that it is only possible to 
create a document collection using the `--create-collection` flag, and no edge
collections.

    unix> arangoimp --file "data.json" --type json --collection "users" --create-collection true

When importing data into an existing collection it is often convenient to first
remove all data from the collection and then start the import. This can be achieved
by passing the `--overwrite` parameter to _arangoimp_. If it is set to `true`,
any existing data in the collection will be removed prior to the import. Note
that any existing index definitions for the collection will be preserved even if 
`--overwrite` is set to true.
    
    unix> arangoimp --file "data.json" --type json --collection "users" --overwrite true

As the import file already contains the data in JSON format, attribute names and
data types are fully preserved. As can be seen in the example data, there is no
need for all data records to have the same attribute names or types. Records can
be in-homogenous.

Please note that by default, _arangoimp_ will import data into the specified 
collection in the default database (`_system`). To specify a different database, 
use the `--server.database` option when invoking _arangoimp_. 

An _arangoimp_ import run will print out the final results on the command line.
By default, it shows the number of documents created, the number of errors that
occurred on the server side, and the total number of input file lines/documents
that it processed. Additionally, _arangoimp_ will print out details about errors 
that happened on the server-side (if any).

Example:

    created:          2
    errors:           0
    total:            2
Please note that _arangoimp_ supports two formats when importing JSON data from 
a file. The first format requires the input file to contain one JSON document
in each line, e.g.

    { "_key": "one", "value": 1 }
    { "_key": "two", "value": 2 }
    { "_key": "foo", "value": "bar" }
    ...

The above format can be imported sequentially by _arangoimp_. It will read data
from the input file in chunks and send it in batches to the server. Each batch
will be about as big as specified in the command-line parameter `--batch-size`.

An alternative is to put one big JSON document into the input file like this:

    [
      { "_key": "one", "value": 1 },
      { "_key": "two", "value": 2 },
      { "_key": "foo", "value": "bar" },
      ...
    ]

This format allows line breaks within the input file as required. The downside 
is that the whole input file will need to be read by _arangoimp_ before it can
send the first batch. This might be a problem if the input file is big. By
default, _arangoimp_ will allow importing such files up to a size of about 16 MB.

If you want to allow your _arangoimp_ instance to use more memory, you may want
to increase the maximum file size by specifying the command-line option
`--batch-size`. For example, to set the batch size to 32 MB, use the following
command:

    unix> arangoimp --file "data.json" --type json --collection "users" --batch-size 33554432

Please also note that you may need to increase the value of `--batch-size` if
a single document inside the input file is bigger than the value of `--batch-size`.


<a name="importing_csv_data"></a>
### Importing CSV Data

_arangoimp_ also offers the possibility to import data from CSV files. This
comes handy when the data at hand is in CSV format already and you don't want to
spend time converting them to JSON for the import.

To import data from a CSV file, make sure your file contains the attribute names
in the first row. All the following lines in the file will be interpreted as
data records and will be imported.

The CSV import requires the data to have a homogeneous structure. All records
must have exactly the same amount of columns as there are headers.

The cell values can have different data types though. If a cell does not have
any value, it can be left empty in the file. These values will not be imported
so the attributes will not "be there" in document created. Values enclosed in
quotes will be imported as strings, so to import numeric values, boolean values
or the null value, don't enclose the value into the quotes in your file.

We'll be using the following import for the CSV import:

@verbinclude arangoimp-data-csv

The command line to execute the import then is:

    unix> arangoimp --file "data.csv" --type csv --collection "users"

Note that the quote and separator characters can be adjusted via the
`--quote` and `--separator` arguments when invoking _arangoimp_.  The importer
supports Windows (CRLF) and Unix (LF) line breaks.

<a name="importing_tsv_data"></a>
### Importing TSV Data

You may also import tab-separated values (TSV) from a file. This format is very
simple: every line in the file represents a data record. There is no quoting or
escaping. That also means that the separator character (which defaults to the
tabstop symbol) must not be used anywhere in the actual data.

As with CSV, the first line in the TSV file must contain the attribute names,
and all lines must have an identical number of values.

If a different separator character or string should be used, it can be specified
with the `--separator` argument. 

An example command line to execute the TSV import is:

    unix> arangoimp --file "data.tsv" --type tsv --collection "users" 

<a name="importing_into_an_edge_collection"></a>
### Importing into an Edge Collection

arangoimp can also be used to import data into an existing edge collection.
The import data must, for each edge to import, contain at least the `_from` and
`_to` attributes. These indicate which other two documents the edge should connect.
It is necessary that these attributes are set for all records, and point to 
valid document ids in existing collections.

Example:

    { "_from" : "users/1234", "_to" : "users/4321", "desc" : "1234 is connected to 4321" }

Note that the edge collection must already exist when the import is started. Using 
the `--create-collection` flag will not work because arangoimp will always try to 
create a regular document collection if the target collection does not exist.

<a name="attribute_naming_and_special_attributes"></a>
### Attribute Naming and Special Attributes

Attributes whose names start with an underscore are treated in a special way by 
ArangoDB:

- the optional `_key` attribute contains the document's key. If specified, the value
  must be formally valid (e.g. must be a string and conform to the naming conventions 
  for @ref DocumentKeys). Additionally, the key value must be unique within the 
  collection the import is run for.
- `_from`: when importing into an edge collection, this attribute contains the id
  of one of the documents connected by the edge. The value of `_from` must be a
  syntactially valid document id and the referred collection must exist.
- `_to`: when importing into an edge collection, this attribute contains the id
  of the other document connected by the edge. The value of `_to` must be a
  syntactially valid document id and the referred collection must exist.
- `_rev`: this attribute contains the revision number of a document. However, the
  revision numbers are managed by ArangoDB and cannot be specified on import. Thus 
  any value in this attribute is ignored on import.
- all other attributes starting with an underscore are discarded on import without
  any warnings.

If you import values into `_key`, you should make sure they are valid and unique.

When importing data into an edge collection, you should make sure that all import
documents can `_from` and `_to` and that their values point to existing documents.

Finally you should make sure that all other attributes in the import file do not
start with an underscore - otherwise they might be discarded.
