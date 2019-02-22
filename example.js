const unveil = require('.')
const fs = require('fs')
const path = require('path')

unveil(__dirname, 'rwc') // can read, write and create files from here down
try {
  fs.openSync(path.resolve(__dirname, '../text.txt'), 'a+', 0o555)
} catch (ex) {
  console.log('failed first time')
}

unveil(path.resolve(__dirname, '../text.txt'), 'rwc') // can read, write and create files from here down
try {
  fs.openSync(path.resolve(__dirname, '../text.txt'), 'a+', 0o555)
} catch (ex) {
  console.log('failed second time')
}
