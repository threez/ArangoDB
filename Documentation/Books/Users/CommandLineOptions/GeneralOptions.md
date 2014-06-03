<a name="general_options"></a>
# General Options

`--help`
`-h`

Prints a list of the most common options available and then exits. In order to see all options use `--help-all`.

`--version`
`-v`

Prints the version of the server and exits.


`--upgrade`

Specifying this option will make the server perform a database upgrade at start. A database upgrade will first compare the version number stored in the file VERSION in the database directory with the current server version.

If the two version numbers match, the server will start normally.

If the version number found in the database directory is higher than the version number the server is running, the server expects this is an unintentional downgrade and will warn about this. It will however start normally. Using the server in these conditions is however not recommended nor supported.

If the version number found in the database directory is lower than the version number the server is running, the server will check whether there are any upgrade tasks to perform. It will then execute all required upgrade tasks and print their statuses. If one of the upgrade tasks fails, the server will exit and refuse to start. Re-starting the server with the upgrade option will then again trigger the upgrade check and execution until the problem is fixed. If all tasks are finished, the server will start normally.

Whether or not this option is specified, the server will always perform a version check on startup. Running the server with a non-matching version number in the VERSION file will make the server refuse to start.


`--configuration filename`
`-c filename`

Specifies the name of the configuration file to use.

If this command is not passed to the server, then by default, the server will attempt to first locate a file named ~/.arango/arangod.conf in the user's home directory.

If no such file is found, the server will proceed to look for a file arangod.conf in the system configuration directory. The system configuration directory is platform-specific, and may be changed when compiling ArangoDB yourself. It may default to /etc/arangodb or /usr/local/etc/arangodb. This file is installed when using a package manager like rpm or dpkg. If you modify this file and later upgrade to a new version of ArangoDB, then the package manager normally warns you about the conflict. In order to avoid these warning for small adjustments, you can put local overrides into a file arangod.conf.local.

Only command line options with a value should be set within the configuration file. Command line options which act as flags should be entered on the command line when starting the server.

Whitespace in the configuration file is ignored. Each option is specified on a separate line in the form

	key = value
 
Alternatively, a header section can be specified and options pertaining to that section can be specified in a shorter form

	[log] 
	level = trace

rather than specifying

	log.level = trace 

Comments can be placed in the configuration file, only if the line begins with one or more hash symbols (#).

There may be occasions where a configuration file exists and the user wishes to override configuration settings stored in a configuration file. Any settings specified on the command line will overwrite the same setting when it appears in a configuration file. If the user wishes to completely ignore configuration files without necessarily deleting the file (or files), then add the command line option

	-c none 
or

	--configuration none 

when starting up the server. Note that, the word none is case-insensitive.

<!--
@anchor CommandLineHelp
@copydetails triagens::rest::ApplicationServer::_options

@CLEARPAGE
@anchor CommandLineVersion
@copydetails triagens::rest::ApplicationServer::_version

@CLEARPAGE
@anchor CommandLineUpgrade
@copydetails triagens::arango::ApplicationV8::_performUpgrade

@CLEARPAGE
@anchor CommandLineConfiguration
@copydetails triagens::rest::ApplicationServer::_configFile

@CLEARPAGE
@anchor CommandLineDaemon
-->

`--daemon`

Runs the server as a daemon (as a background process). This parameter can only
be set if the pid (process id) file is specified. That is, unless a value to the
parameter pid-file is given, then the server will report an error and exit.

--default-language default-language
The default language ist used for sorting and comparing strings. The language value is a two-letter language code (ISO-639) or it is composed by a two-letter language code with and a two letter country code (ISO-3166). Valid languages are "de", "en", "en_US" or "en_UK".

The default default-language is set to be the system locale on that platform.

`--supervisor`

Executes the server in supervisor mode. In the event that the server
unexpectedly terminates due to an internal error, the supervisor will
automatically restart the server. Setting this flag automatically implies that
the server will run as a daemon. Note that, as with the daemon flag, this flag
requires that the pid-file parameter will set.

    unix> ./arangod --supervisor --pid-file /var/run/arangodb.pid /tmp/vocbase/
    2012-06-27T15:58:28Z [10133] INFO starting up in supervisor mode

As can be seen (e.g. by executing the ps command), this will start a supervisor
process and the actual database process:

    unix> ps fax | grep arangod
    10137 ?        Ssl    0:00 ./arangod --supervisor --pid-file /var/run/arangodb.pid /tmp/vocbase/
    10142 ?        Sl     0:00  \_ ./arangod --supervisor --pid-file /var/run/arangodb.pid /tmp/vocbase/

When the database process terminates unexpectedly, the supervisor process will
start up a new database process:

    > kill -SIGSEGV 10142

    > ps fax | grep arangod
    10137 ?        Ssl    0:00 ./arangod --supervisor --pid-file /var/run/arangodb.pid /tmp/vocbase/
    10168 ?        Sl     0:00  \_ ./arangod --supervisor --pid-file /var/run/arangodb.pid /tmp/vocbase/

`--uid uid`

The name (identity) of the user the server will run as. If this parameter is not specified, the server will not attempt to change its UID, so that the UID used by the server will be the same as the UID of the user who started the server. If this parameter is specified, then the server will change its UID after opening ports and reading configuration files, but before accepting connections or opening other files (such as recovery files). This is useful when the server must be started with raised privileges (in certain environments) but security considerations require that these privileges be dropped once the server has started work.

Observe that this parameter cannot be used to bypass operating system security. In general, this parameter (and its corresponding relative gid) can lower privileges but not raise them.


`--gid gid`

The name (identity) of the group the server will run as. If this parameter is not specified, then the server will not attempt to change its GID, so that the GID the server runs as will be the primary group of the user who started the server. If this parameter is specified, then the server will change its GID after opening ports and reading configuration files, but before accepting connections or opening other files (such as recovery files).

This parameter is related to the parameter uid.


`--pid-file filename`

The name of the process ID file to use when running the server as a daemon. This parameter must be specified if either the flag daemon or supervisor is set.


`--console`

Runs the server in an exclusive emergency console mode. When starting the server with this option, the server is started with an interactive JavaScript emergency console, with all networking and HTTP interfaces of the server disabled.

No requests can be made to the server in this mode, and the only way to work with the server in this mode is by using the emergency console. Note that the server cannot be started in this mode if it is already running in this or another mode.

<!--
@CLEARPAGE
@anchor CommandLineUid
@copydetails triagens::rest::ApplicationServer::_uid

@CLEARPAGE
@anchor CommandLineGid
@copydetails triagens::rest::ApplicationServer::_gid

@CLEARPAGE
@anchor CommandLinePidFile
@copydetails triagens::rest::AnyServer::_pidFile

@CLEARPAGE
@anchor CommandLineConsole
@CMDOPT{\--console}
-->
	
Runs the server in an exclusive emergency console mode. When 
starting the server with this option, the server is started with
an interactive JavaScript emergency console, with all networking
and HTTP interfaces of the server disabled.

No requests can be made to the server in this mode, and the only
way to work with the server in this mode is by using the emergency
console. 
Note that the server cannot be started in this mode if it is 
already running in this or another mode. 