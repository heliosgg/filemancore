// Test namespace FileManCore::Utils::FileSystem

import { FileInfo, listDir, listDrives } from "../../lib/index";
import { expect } from "chai";
import { mkdirSync, rmdirSync, writeFileSync } from "fs";

describe('FileSystem functions', () => {
    before(() => {
        mkdirSync('.\\test_folder_1\\some\\path', { recursive: true });
        mkdirSync('.\\test_folder_1\\another', { recursive: true });
        //writeFileSync('.\\test_folder_1\\for.mask', 'hello');
        //writeFileSync('.\\test_folder_1\\forfor.mask', 'qq');
        //writeFileSync('.\\test_folder_1\\withoutMask', 'opta');
    });

    after(() => {
        rmdirSync('.\\test_folder_1', { recursive: true });
    });
    
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

            expect(l.length).to.be.equal(3);

            expect(l[1].attributes.directory).to.be.true;
            expect(l[1].attributes.hidden).to.be.false;
            expect(l[1].attributes.readonly).to.be.false;
            expect(l[1].attributes.system).to.be.false;
            expect(l[1].size).to.be.equal(0);

            expect(l[2].attributes.directory).to.be.true;
            expect(l[2].attributes.hidden).to.be.false;
            expect(l[2].attributes.readonly).to.be.false;
            expect(l[2].attributes.system).to.be.false;
            expect(l[2].size).to.be.equal(0);

            // TODO: check dir names
        });

        it('should work with masks', () => {
            // TODO: do this
        });
    });
});