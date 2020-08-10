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

function listDrives(): string[] {
  return fmCore.listDrives();
}

function normalizePath(path: string): string {
  return fmCore.normalizePath(path);
}

function moveFile(existingFileName: string, newFileName: string): boolean {
  return fmCore.moveFile(existingFileName, newFileName);
}

function openWithDefaultApp(filePath: string): boolean {
  return fmCore.openWithDefaultApp(filePath);
}

function getRemoteCWD(pid: number): string {
  return fmCore.getRemoteCWD(pid);
}

export { listDir, listDrives, normalizePath, moveFile, openWithDefaultApp, getRemoteCWD, FileInfo };
