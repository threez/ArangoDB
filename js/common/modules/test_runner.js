/*jslint indent: 2, nomen: true, maxlen: 120, regexp: true, todo: true */
/*global module, require, exports, print */

var runTest = require('jsunity').runTest,
  _ = require('underscore'),
  internal = require('internal'),
  runJSUnityTests,
  runJasmineTests,
  runCommandLineTests;

////////////////////////////////////////////////////////////////////////////////
/// @brief runs all jsunity tests
////////////////////////////////////////////////////////////////////////////////

runJSUnityTests = function (tests) {
  'use strict';
  var result = true;

  _.each(tests, function (file) {
    print("\nRunning JSUnity test from file '" + file + "'");

    try {
      result = result && runTest(file);
    } catch (err) {
      print("cannot run test file '" + file + "': " + err);
      print(err.stack);
      result = false;
    }

    internal.wait(0); // force GC
  });

  return result;
};

////////////////////////////////////////////////////////////////////////////////
/// @brief runs all jsunity tests
////////////////////////////////////////////////////////////////////////////////

runJasmineTests = function (testFiles, options) {
  'use strict';
  var result = true;

  if (testFiles.length > 0) {
    print('\nRunning Jasmine Tests: ' + testFiles.join(', '));
    result = require('jasmine').executeTestSuite(testFiles, options);
  }

  return result;
};

////////////////////////////////////////////////////////////////////////////////
/// @brief runs tests from command-line
////////////////////////////////////////////////////////////////////////////////

runCommandLineTests = function (opts) {
  'use strict';
  var result = true,
    options = opts || {},
    jasmineReportFormat = options.jasmineReportFormat || 'progress',
    unitTests = internal.unitTests(),
    isSpecRegEx = /.+spec.js/,
    isSpec = function (unitTest) {
      return isSpecRegEx.test(unitTest);
    },
    jasmine = _.filter(unitTests, isSpec),
    jsUnity = _.reject(unitTests, isSpec);

  result = runJSUnityTests(jsUnity) && runJasmineTests(jasmine, { format: jasmineReportFormat });

  internal.setUnitTestsResult(result);
};

exports.runCommandLineTests = runCommandLineTests;