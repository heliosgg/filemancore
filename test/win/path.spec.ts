// Test namespace FileManCore::Utils::Path

import { normalizePath } from "../../lib/index";
import { expect } from "chai";
import { mkdirSync, rmdirSync } from "fs";
import { cwd } from "process";

describe("Path functions", () => {
    describe("normalizePath", () => {
        before(() => {
            mkdirSync('.\\test_folder_0\\very\\long\\path\\bla\\bla\\lol', { recursive: true });
        });
    
        after(() => {
            rmdirSync('.\\test_folder_0', { recursive: true });
        });

        let curDir = cwd();

        it('should expand leading dot', () => {
            expect(normalizePath('.\\test_folder_0\\very\\'))
                .to.equal(curDir + '\\test_folder_0\\very\\');
        });

        it('should collapse double dot', () => {
            expect(normalizePath('.\\test_folder_0\\very\\long\\kek\\..\\path'))
                .to.equal(curDir + '\\test_folder_0\\very\\long\\path');
        });

        it('should collapse trailing double dot', () => {
            expect(normalizePath('.\\test_folder_0\\very\\long\\path\\bla\\bla\\lol\\..'))
                .to.equal(curDir + '\\test_folder_0\\very\\long\\path\\bla\\bla');
        });

        it('should collapse a lot of double dot', () => {
            expect(normalizePath('.\\test_folder_0\\very\\long\\kek\\bla\\lol\\..\\..\\..\\path'))
                .to.equal(curDir + '\\test_folder_0\\very\\long\\path');
        });

        it('should remove a lot of useless slashes', () => {
            expect(normalizePath('.\\test_folder_0\\\\\\\\\\very\\\\\\\\long\\\\path\\bla\\bla\\\\lol'))
                .to.equal(curDir + '\\test_folder_0\\very\\long\\path\\bla\\bla\\lol');
        });

        it('should flip forward slashes to backslashes', () => {
            expect(normalizePath('./test_folder_0\\very/long\\path/bla\\bla/lol'))
                .to.equal(curDir + '\\test_folder_0\\very\\long\\path\\bla\\bla\\lol');
        });
    });
});