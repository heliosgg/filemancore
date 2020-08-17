// Test namespace FileManCore::Utils::FileSystem

import { FileInfo, listDir, listDrives, moveFile } from "../../lib/index";
import { expect, assert } from "chai";
import { mkdirSync, rmdirSync, writeFileSync, existsSync, readFileSync } from "fs";
import { cwd } from "process";

describe('FileSystem functions', () => {
    before(() => {
        mkdirSync('.\\test_folder_1\\some\\path', { recursive: true });
        mkdirSync('.\\test_folder_1\\another', { recursive: true });
        writeFileSync('.\\test_folder_1\\for.mask', 'hello');
        writeFileSync('.\\test_folder_1\\forfor.mask', 'qq');
        writeFileSync('.\\test_folder_1\\withoutMask', 'opta');
        writeFileSync('.\\test_folder_1\\move.me', 'bruh');
    });

    after(() => {
        rmdirSync('.\\test_folder_1', { recursive: true });
    });
    
    let curDir = cwd();

    describe('listDrives', ()=>{
        it('should return something', () => {
            expect(listDrives().length).to.be.greaterThan(0);
        });
    });

    describe('listDir', () => {
        it('should return something', () => {
            expect(listDir('.\\test_folder_1').length).to.be.gte(1);
        });
        
        it('should return some and another directory', () => {
            let l = listDir('.\\test_folder_1');

            l.every(x => expect(x).to.have.own.property('path', curDir + "\\test_folder_1\\" ));
            l.every(x => expect(x).to.have.nested.property('attributes.hidden', false ));
            l.every(x => expect(x).to.have.nested.property('attributes.readonly', false ));
            l.every(x => expect(x).to.have.nested.property('attributes.system', false ));

            let dirs = l.filter(x => x.attributes.directory);
            dirs.every(x => expect(x).to.have.own.property('size', 0));

            let dirNames = dirs.map(x => x.name);
            expect(dirNames).to.contain('..');
            expect(dirNames).to.contain('some');
            expect(dirNames).to.contain('another');

            let files = l.filter(x => x.attributes.directory == false);
            files.every(x => expect(x).to.have.own.property('size').and.to.not.equal(0));

            let fileNames = files.map(x => x.name);
            expect(fileNames).to.contain('for.mask');
            expect(fileNames).to.contain('forfor.mask');
            expect(fileNames).to.contain('withoutMask');
        });

        it('should work with masks', () => {
            let l = listDir('.\\test_folder_1\\*.mask');

            l.every(x => expect(x).to.not.have.nested.property('attributes.directory', true ));

            let names = l.map(x => x.name);
            expect(names).to.contain('for.mask');
            expect(names).to.contain('forfor.mask');
            expect(names).not.to.contain('withoutMask');
        });
    });

    describe("moveFile", () => {
        it('should move files', () => {
            expect(existsSync('.\\test_folder_1\\move.me')).to.be.true;
            expect(moveFile('.\\test_folder_1\\move.me', '.\\test_folder_1\\some\\path\\new.name')).to.be.true;
            expect(existsSync('.\\test_folder_1\\move.me')).to.be.false;
            expect(existsSync('.\\test_folder_1\\some\\path\\new.name')).to.be.true;
            expect(readFileSync('.\\test_folder_1\\some\\path\\new.name').toString()).to.be.equal('bruh');
        });
    });
});