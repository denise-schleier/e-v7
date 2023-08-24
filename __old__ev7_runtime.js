var __old__ev7_root = {
    _init: init,

    push: push,
    dump: dump
};

function __old__init() {
    this.v = []
}


function __old__push(v) {
    this.v.push(v)

    return this
}

function __old__dump() {
    this.v.forEach(function (e, index) {print(index, ':', e)})
    this.v = []

    return this
}


__old__ev7_root._init()

function __old__ev7() {
    return ev7_root;
}
