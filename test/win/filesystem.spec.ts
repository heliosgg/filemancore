// Test namespace FileManCore::Utils::FileSystem

import { FileInfo, listDir, listDrives, moveFile, createEmptyFile } from "../../lib/index";
import { expect } from "chai";
import { mkdirSync, rmdirSync, writeFileSync, existsSync, readFileSync, statSync } from "fs";
import { cwd } from "process";

describe('FileSystem functions', () => {
    let currentTestFolder = '.\\test_folder_1';

    before(() => {
        mkdirSync(currentTestFolder + '\\some\\path', { recursive: true });
        mkdirSync(currentTestFolder + '\\another', { recursive: true });
        writeFileSync(currentTestFolder + '\\for.mask', 'hello');
        writeFileSync(currentTestFolder + '\\forfor.mask', 'qq');
        writeFileSync(currentTestFolder + '\\withoutMask', 'opta');
        writeFileSync(currentTestFolder + '\\move.me', 'bruh');
    });

    after(() => {
        rmdirSync(currentTestFolder, { recursive: true });
    });
    
    let curDir = cwd();

    describe('listDrives', ()=>{
        it('should return something', () => {
            expect(listDrives().length).to.be.greaterThan(0);
        });
    });

    describe('listDir', () => {
        it('should return something', () => {
            expect(listDir(currentTestFolder).length).to.be.gte(1);
        });
        
        it('should return some and another directory', () => {
            let l = listDir(currentTestFolder);

            l.every(x => expect(x).to.have.own.property('path', curDir + currentTestFolder.substr(1) + '\\' ));
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
            let l = listDir(currentTestFolder + '\\*.mask');

            l.every(x => expect(x).to.not.have.nested.property('attributes.directory', true ));

            let names = l.map(x => x.name);
            expect(names).to.contain('for.mask');
            expect(names).to.contain('forfor.mask');
            expect(names).not.to.contain('withoutMask');
        });
    });

    describe('moveFile', () => {
        it('should move files', () => {
            expect(existsSync(currentTestFolder + '\\move.me')).to.be.true;
            expect(moveFile(currentTestFolder + '\\move.me', currentTestFolder + '\\some\\path\\new.name')).to.be.true;
            expect(existsSync(currentTestFolder + '\\move.me')).to.be.false;
            expect(existsSync(currentTestFolder + '\\some\\path\\new.name')).to.be.true;
            expect(readFileSync(currentTestFolder + '\\some\\path\\new.name').toString()).to.be.equal('bruh');
        });
    });


    describe('createEmptyFile', () => {
        it('should create empty file', () => {
            let fileToCreate = currentTestFolder + '\\createEmptyFile.txt';

            expect(existsSync(fileToCreate)).to.be.false;
            expect(createEmptyFile(fileToCreate, false)).to.be.true;
            expect(existsSync(fileToCreate)).to.be.true;
            expect(statSync(fileToCreate).size).to.be.equal(0);
        });
        
        it('should erase existing file, if eraseIfExist true', () => {
            let fileToCreate = currentTestFolder + '\\eraseNotEmptyFile.txt';

            expect(existsSync(fileToCreate)).to.be.false;
            writeFileSync(fileToCreate, 'hello, world');
            expect(existsSync(fileToCreate)).to.be.true;
            expect(statSync(fileToCreate).size).not.to.be.equal(0);

            expect(createEmptyFile(fileToCreate, true)).to.be.true;
            expect(existsSync(fileToCreate)).to.be.true;
            expect(statSync(fileToCreate).size).to.be.equal(0);
        });
    });
});