// Test namespace FileManCore::Utils::OS

import { openWithDefaultApp, getRemoteCWD } from "../../lib/index";
import { expect } from "chai";
import { cwd, pid } from "process";
import { mkdirSync, rmdirSync, writeFileSync } from "fs";

describe('OS functions', () => {
    before(() => {
        mkdirSync('.\\test_folder_2');
        writeFileSync('.\\test_folder_2\\random_file.bat', 'exit /b 0\n');
    });

    after(() => {
        rmdirSync('.\\test_folder_2', { recursive: true });
    });

    let curDir = cwd();
    let curPid = pid;

    describe('openWithDefaultApp', () => {
        it('should not fail', () => {
            expect(openWithDefaultApp('.\\test_folder_2\\random_file.bat')).to.be.true;
        });
    });

    describe('getRemoteCWD', () => {
        it('should get current working directory', () => {
            expect(getRemoteCWD(curPid)).to.be.equal(curDir + '\\');
        });
    });
});