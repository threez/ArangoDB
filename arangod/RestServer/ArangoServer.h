////////////////////////////////////////////////////////////////////////////////
/// @brief arango server
///
/// @file
///
/// DISCLAIMER
///
/// Copyright 2004-2012 triAGENS GmbH, Cologne, Germany
///
/// Licensed under the Apache License, Version 2.0 (the "License");
/// you may not use this file except in compliance with the License.
/// You may obtain a copy of the License at
///
///     http://www.apache.org/licenses/LICENSE-2.0
///
/// Unless required by applicable law or agreed to in writing, software
/// distributed under the License is distributed on an "AS IS" BASIS,
/// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
/// See the License for the specific language governing permissions and
/// limitations under the License.
///
/// Copyright holder is triAGENS GmbH, Cologne, Germany
///
/// @author Dr. Frank Celler
/// @author Copyright 2011-2012, triAGENS GmbH, Cologne, Germany
////////////////////////////////////////////////////////////////////////////////

#ifndef TRIAGENS_REST_SERVER_ARANGO_SERVER_H
#define TRIAGENS_REST_SERVER_ARANGO_SERVER_H 1

#include "Rest/AnyServer.h"
#include "Rest/EndpointList.h"

#include "VocBase/vocbase.h"

// -----------------------------------------------------------------------------
// --SECTION--                                              forward declarations
// -----------------------------------------------------------------------------

namespace triagens {
  namespace rest {
    class ApplicationDispatcher;
    class ApplicationHttpServer;
    class ApplicationHttpsServer;
    class ApplicationScheduler;
#ifdef TRI_ENABLE_ZEROMQ
    class ApplicationZeroMQ;
#endif 
    class HttpServer;
  }

  namespace admin {
    class ApplicationUserManager;
    class ApplicationAdminServer;
  }

  namespace arango {
    class ApplicationMR;
    class ApplicationV8;

// -----------------------------------------------------------------------------
// --SECTION--                                                class ArangoServer
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ArangoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
/// @brief ArangoDB server shell operation modes
////////////////////////////////////////////////////////////////////////////////

    typedef enum {
      MODE_CONSOLE,
      MODE_UNITTESTS,
      MODE_JSLINT,
    }
    server_operation_mode_e;


////////////////////////////////////////////////////////////////////////////////
/// @brief ArangoDB server
////////////////////////////////////////////////////////////////////////////////

  class ArangoServer : public rest::AnyServer {
      private:
        ArangoServer (const ArangoServer&);
        ArangoServer& operator= (const ArangoServer&);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                      constructors and destructors
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ArangoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// @brief UnviversalVoc constructor
////////////////////////////////////////////////////////////////////////////////

        ArangoServer (int argc, char** argv);

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 AnyServer methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ArangoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

      public:

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        void buildApplicationServer ();

////////////////////////////////////////////////////////////////////////////////
/// {@inheritDoc}
////////////////////////////////////////////////////////////////////////////////

        int startupServer ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                   private methods
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ArangoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief executes the JavaScript emergency console
////////////////////////////////////////////////////////////////////////////////

        int executeConsole (server_operation_mode_e);

////////////////////////////////////////////////////////////////////////////////
/// @brief executes the ruby emergency console
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_ENABLE_MRUBY
        int executeRubyConsole ();
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief opens the database
////////////////////////////////////////////////////////////////////////////////

        void openDatabase ();

////////////////////////////////////////////////////////////////////////////////
/// @brief closes the database
////////////////////////////////////////////////////////////////////////////////

        void closeDatabase ();

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

// -----------------------------------------------------------------------------
// --SECTION--                                                 private variables
// -----------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////
/// @addtogroup ArangoDB
/// @{
////////////////////////////////////////////////////////////////////////////////

      private:

////////////////////////////////////////////////////////////////////////////////
/// @brief number of command line arguments
////////////////////////////////////////////////////////////////////////////////

        int _argc;

////////////////////////////////////////////////////////////////////////////////
/// @brief command line arguments
////////////////////////////////////////////////////////////////////////////////

        char** _argv;

////////////////////////////////////////////////////////////////////////////////
/// @brief path to binary
////////////////////////////////////////////////////////////////////////////////

        std::string _binaryPath;

////////////////////////////////////////////////////////////////////////////////
/// @brief application scheduler
////////////////////////////////////////////////////////////////////////////////

        rest::ApplicationScheduler* _applicationScheduler;

////////////////////////////////////////////////////////////////////////////////
/// @brief application dispatcher
////////////////////////////////////////////////////////////////////////////////

        rest::ApplicationDispatcher* _applicationDispatcher;

////////////////////////////////////////////////////////////////////////////////
/// @brief application http server
////////////////////////////////////////////////////////////////////////////////

        rest::ApplicationHttpServer* _applicationHttpServer;

////////////////////////////////////////////////////////////////////////////////
/// @brief application https server
////////////////////////////////////////////////////////////////////////////////

        rest::ApplicationHttpsServer* _applicationHttpsServer;

////////////////////////////////////////////////////////////////////////////////
/// @brief constructed admin server application
////////////////////////////////////////////////////////////////////////////////

        admin::ApplicationAdminServer* _applicationAdminServer;

////////////////////////////////////////////////////////////////////////////////
/// @brief constructed user server application
////////////////////////////////////////////////////////////////////////////////

        admin::ApplicationUserManager* _applicationUserManager;

////////////////////////////////////////////////////////////////////////////////
/// @brief application MR
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_ENABLE_MRUBY
        ApplicationMR* _applicationMR;
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief application V8
////////////////////////////////////////////////////////////////////////////////

        ApplicationV8* _applicationV8;

////////////////////////////////////////////////////////////////////////////////
/// @brief ZeroMQ server
////////////////////////////////////////////////////////////////////////////////

#ifdef TRI_ENABLE_ZEROMQ
        rest::ApplicationZeroMQ* _applicationZeroMQ;
#endif

////////////////////////////////////////////////////////////////////////////////
/// @brief constructed http server
////////////////////////////////////////////////////////////////////////////////

        rest::HttpServer* _httpServer;

////////////////////////////////////////////////////////////////////////////////
/// @brief constructed admin http server
////////////////////////////////////////////////////////////////////////////////

        rest::HttpServer* _adminHttpServer;

////////////////////////////////////////////////////////////////////////////////
/// @brief endpoint list container
////////////////////////////////////////////////////////////////////////////////

        rest::EndpointList _endpointList;

////////////////////////////////////////////////////////////////////////////////
/// @brief endpoints for client HTTP requests
///
/// @CMDOPT{--server.endpoint @CA{endpoint}}
///
/// Specifies an @CA{endpoint} for HTTP requests by clients. Endpoints have
/// the following pattern:
/// - tcp://[ipv6-address]:port - TCP/IP endpoint, using IPv6
/// - tcp://ipv4-address:port - TCP/IP endpoint, using IPv4
/// - unix:///path/to/socket - Unix domain socket endpoint
///
/// If a TCP/IP endpoint is specified without a port number, then the default 
/// port (8529) will be used.
/// If multiple endpoints need to be used, the option can be repeated multiple
/// times.
////////////////////////////////////////////////////////////////////////////////

        vector<string> _endpoints;

////////////////////////////////////////////////////////////////////////////////
/// @brief list port for client requests
///
/// @CMDOPT{--server.http-port @CA{port}}
///
/// Specifies the @CA{port} for HTTP requests by clients. This will bind to any
/// address available. If you do not specify an admin port, then the http port
/// will serve both client and administration request. If you have
/// higher security requirements, you can use a special administration
/// port.
///
/// @CMDOPT{--server.http-port @CA{address}:@CA{port}}
///
/// Specifies the @CA{address} and @CA{port} for HTTP requests by clients. This
/// will bind to the given @CA{address}, which can be a numeric value like
/// @CODE{192.168.1.1} or a name.
///
/// @CMDOPT{--port @CA{port}}
///
/// This variant can be used as command line option.
////////////////////////////////////////////////////////////////////////////////

        string _httpPort;

////////////////////////////////////////////////////////////////////////////////
/// @brief number of dispatcher threads for non-database worker
///
/// @CMDOPT{--server.threads @CA{number}}
///
/// Specifies the @CA{number} of threads that are spawned to handle action
/// requests using Rest, JavaScript, or Ruby.
////////////////////////////////////////////////////////////////////////////////

        int _dispatcherThreads;

////////////////////////////////////////////////////////////////////////////////
/// @brief path to the database
///
/// @CMDOPT{--database.directory @CA{directory}}
///
/// The directory containing the collections and data-files. Defaults
/// to @CODE{/var/lib/arango}.
///
/// @CMDOPT{@CA{directory}}
///
/// When using the command line version, you can simply supply the database
/// directory as argument.
///
/// @EXAMPLES
///
/// @verbinclude option-database-directory
////////////////////////////////////////////////////////////////////////////////

        string _databasePath;

////////////////////////////////////////////////////////////////////////////////
/// @brief remove on drop
///
/// @CMDOPT{--database.remove-on-drop @CA{flag}}
///
/// If @LIT{true} and you drop a collection, then they directory and all
/// associated datafiles will be removed from disk. If @LIT{false}, then they
/// collection directory will be renamed to @LIT{deleted-...}, but remains on
/// hard disk. To restore such a dropped collection, you can rename the
/// directory back to @LIT{collection-...}, but you must also edit the file
/// @LIT{parameter.json} inside the directory.
///
/// The default is @LIT{true}.
////////////////////////////////////////////////////////////////////////////////

        bool _removeOnDrop;

////////////////////////////////////////////////////////////////////////////////
/// @brief remove on compaction
///
/// @CMDOPT{--database.remove-on-compaction @CA{flag}}
///
/// Normally the garbage collection will removed compacted datafile. For debug
/// purposes you can use this option to keep the old datafiles. You should
/// never set it to @LIT{false} on a live system.
///
/// The default is @LIT{true}.
////////////////////////////////////////////////////////////////////////////////

        bool _removeOnCompacted;

////////////////////////////////////////////////////////////////////////////////
/// @brief default journal size
///
/// @CMDOPT{--database.maximal-journal-size @CA{size}}
///
/// Maximal size of journal in bytes. Can be overwritten when creating a new
/// collection. Note that this also limits the maximal size of a single
/// document.
///
/// The default is @LIT{32MB}.
////////////////////////////////////////////////////////////////////////////////

        uint64_t _defaultMaximalSize;

////////////////////////////////////////////////////////////////////////////////
/// @brief unit tests
///
/// @CMDOPT{--unit-tests @CA{test-file}}
///
/// Runs one or more unit tests.
////////////////////////////////////////////////////////////////////////////////

        vector<string> _unitTests;

////////////////////////////////////////////////////////////////////////////////
/// @brief files to jslint
///
/// @CMDOPT{--jslint @CA{test-file}}
///
/// Runs jslint on one or more files.
////////////////////////////////////////////////////////////////////////////////

        vector<string> _jslint;

////////////////////////////////////////////////////////////////////////////////
/// @brief vocbase
////////////////////////////////////////////////////////////////////////////////

        TRI_vocbase_t* _vocbase;
    };
  }
}

////////////////////////////////////////////////////////////////////////////////
/// @}
////////////////////////////////////////////////////////////////////////////////

#endif

// -----------------------------------------------------------------------------
// --SECTION--                                                       END-OF-FILE
// -----------------------------------------------------------------------------

// Local Variables:
// mode: outline-minor
// outline-regexp: "^\\(/// @brief\\|/// {@inheritDoc}\\|/// @addtogroup\\|/// @page\\|// --SECTION--\\|/// @\\}\\)"
// End:
