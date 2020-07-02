"use strict";

Object.defineProperty(exports, "__esModule", { value: true });

var fmCore;
if (process.platform === 'win32') {
    fmCore = require('../build/Release/filemancore.node');
}
else {
    fmCore = null;
}

function listDir(path) {
    return fmCore.listDir(path);
}
exports.listDir = listDir;

function normalizePath(path) {
    return fmCore.normalizePath(path);
}
exports.normalizePath = normalizePath;