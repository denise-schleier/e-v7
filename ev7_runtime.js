
var console = {
    log(a, b, c, d, e, f) {
        print(a, b, c, d, e, f)
    }
}



var fs = {
    readFileSync(filename) {
        return 'duda'
    }
}


var exports = {
    console: console,
    fs: fs
}

function ev7() {
    return exports;
}
