const unveil = require('.')
const fs = require('fs')

unveil(__dirname, 'rwc') // can read, write and create files from here down
const fd = fs.openSync('../test.txt', 'r', 0o555)
