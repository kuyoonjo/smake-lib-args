const { LLVM_Darwin, LLVM_Win32, LLVM_Linux } = require('smake');
const { addLibs } = require('@smake/libs');
const { args } = require('./lib');

class darwin_arm64 extends LLVM_Darwin {
  ARCH = 'arm64';
  name = this.target;
  cxflags = [
    ...super.cxflags,
    '-O3',
    '-std=c++17',
  ];

  files = ['src/args.cc'];
}

class linux_x64 extends LLVM_Linux {
  name = this.target;
  cxflags = [
    ...super.cxflags,
    '-O3',
    '-std=c++17',
  ];

  files = ['src/args.cc'];
}

class win_x64 extends LLVM_Win32 {
  name = this.target;
  cxflags = [
    ...super.cxflags,
    '-O3',
    '-std=c++17',
  ];

  files = ['src/args.cc'];
}

module.exports = {
  targets: [
    addLibs(darwin_arm64, args),
    addLibs(linux_x64, args),
    addLibs(win_x64, args),
  ],
};
