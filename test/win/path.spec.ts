// Test namespace FileManCore::Utils::Path

import { normalizePath } from "../../lib/index";
import { expect } from "chai";
import { mkdirSync, rmdirSync } from "fs";
import { cwd } from "process";

describe('Path functions', () => {
    describe('normalizePath', () => {
        let currentTestFolderWithoutDot = '\\test_folder_0';
        let currentTestFolder = '.' + currentTestFolderWithoutDot;

        before(() => {
            mkdirSync(currentTestFolder + '\\very\\long\\path\\bla\\bla\\lol', { recursive: true });
        });
    
        after(() => {
            rmdirSync(currentTestFolder, { recursive: true });
        });

        let curDir = cwd();
        let cwdCurrentTestFolder = curDir + currentTestFolderWithoutDot;

        it('should expand leading dot', () => {
            expect(normalizePath(currentTestFolder + '\\very\\'))
                .to.equal(cwdCurrentTestFolder + '\\very\\');
        });

        it('should collapse double dot', () => {
            expect(normalizePath(currentTestFolder + '\\very\\long\\kek\\..\\path'))
                .to.equal(cwdCurrentTestFolder + '\\very\\long\\path');
        });

        it('should collapse trailing double dot', () => {
            expect(normalizePath(currentTestFolder + '\\very\\long\\path\\bla\\bla\\lol\\..'))
                .to.equal(cwdCurrentTestFolder + '\\very\\long\\path\\bla\\bla');
        });

        it('should collapse a lot of double dot', () => {
            expect(normalizePath(currentTestFolder + '\\very\\long\\kek\\bla\\lol\\..\\..\\..\\path'))
                .to.equal(cwdCurrentTestFolder + '\\very\\long\\path');
        });

        it('should remove a lot of useless slashes', () => {
            expect(normalizePath(currentTestFolder + '\\\\\\\\\\very\\\\\\\\long\\\\path\\bla\\bla\\\\lol'))
                .to.equal(cwdCurrentTestFolder + '\\very\\long\\path\\bla\\bla\\lol');
        });

        it('should flip forward slashes to backslashes', () => {
            expect(normalizePath('./'+ currentTestFolder.substr(2) + '\\very/long\\path/bla\\bla/lol'))
                .to.equal(cwdCurrentTestFolder + '\\very\\long\\path\\bla\\bla\\lol');
        });
    });
});