const { LLVM_Darwin, LLVM_Win32, LLVM_Linux } = require('smake');

class darwin_arm64 extends LLVM_Darwin {
  ARCH = 'arm64';
  name = this.target;
  includedirs = [
    ...super.includedirs,
    './include',
  ];
  cxflags = [
    ...super.cxflags,
    '-O3',
    '-std=c++17',
  ];

  files = ['src/args.cc'];
}

module.exports = {
  targets: [
    darwin_arm64,
  ],
};
