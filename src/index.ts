import { FileInfo } from './interfaces';

var fmCore: any;
if (process.platform === 'win32') {
  fmCore = require('../build/Release/filemancore.node');
} else {
  fmCore = null;
}

function listDir(path: string): FileInfo[] {
  return fmCore.listDir(path);
}

function normalizePath(path: string): string {
  return fmCore.normalizePath(path);
}

export { listDir, normalizePath };
