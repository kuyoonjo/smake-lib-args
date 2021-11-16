const { LLVM } = require('smake');
const { LibArgs } = require('./lib');

const test = new LLVM('test', 'arm64-apple-darwin');
test.files = ['src/args.cc'];
LibArgs.config(test);

module.exports = [test];
