var fs = require('fs');
var path = require('path');

var RELEASE_DIR = path.join(__dirname, '..', 'build', 'Release');
var BUILD_FILES = [
  path.join(RELEASE_DIR, 'filemancore.node'),
  path.join(RELEASE_DIR, 'filemancore.pdb'),
];

cleanFolderRecursive = function(folder) {
  var files = [];
  if( fs.existsSync(folder) ) {
    files = fs.readdirSync(folder);
    files.forEach(function(file,index) {
      var curPath = path.join(folder, file);
      if(fs.lstatSync(curPath).isDirectory()) { // recurse
        cleanFolderRecursive(curPath);
        fs.rmdirSync(curPath);
      } else if (BUILD_FILES.indexOf(curPath) < 0){ // delete file
        fs.unlinkSync(curPath);
      }
    });
  }
};

try {
  cleanFolderRecursive(RELEASE_DIR);
} catch(e) {
  console.log(e);
  //process.exit(1);
} finally {
  process.exit(0);
}