# `openbsd-unveil`

> OpenBSD unveil(2) bindings for Node.js with prebuilds

## Usage

```js
const unveil = require('openbsd-unveil')
const fs = require('fs')

unveil(__dirname, 'rwc') // can read, write and create files from here down
const fd = fs.openSync('../test.txt', 'a+', 0o555)

unveil() // Disable unveil
```

## API

### `unveil([path], [permissions])`

Unveil `path` with `permissions`, both being `string` or `null` to disable
`unveil`.

Please refer to the [`unveil(2)` docs](https://man.openbsd.org/unveil.2)

## Install

```sh
npm install openbsd-unveil
```

## License

[ISC](LICENSE)
