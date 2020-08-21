// Test namespace FileManCore::Utils::OS

import { openWithDefaultApp, getRemoteCWD } from "../../lib/index";
import { expect } from "chai";
import { cwd, pid } from "process";
import { mkdirSync, rmdirSync, writeFileSync } from "fs";

describe('OS functions', () => {
    let currentTestFolder = '.\\test_folder_2';

    before(() => {
        mkdirSync(currentTestFolder);
        writeFileSync(currentTestFolder + '\\random_file.bat', 'exit /b 0\n');
    });

    after(() => {
        rmdirSync(currentTestFolder, { recursive: true });
    });

    let curDir = cwd();
    let curPid = pid;

    describe('openWithDefaultApp', () => {
        it('should not fail', () => {
            expect(openWithDefaultApp(currentTestFolder + '\\random_file.bat')).to.be.true;
        });
    });

    describe('getRemoteCWD', () => {
        it('should get current working directory', () => {
            expect(getRemoteCWD(curPid)).to.be.equal(curDir + '\\');
        });
    });
});