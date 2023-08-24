var ev7_root = {
    _init: init,

    push: push,
    dump: dump
};

function init() {
    this.v = []
}


function push(v) {
    this.v.push(v)

    return this
}

function dump() {
    this.v.forEach(function (e, index) {print(index, ':', e)})
    this.v = []

    return this
}


ev7_root._init()

function ev7() {
    return ev7_root;
}
