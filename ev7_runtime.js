var ev7_root = /** @class */ (function () {
    function ev7_root() {
        var args = [];
        for (var _i = 0; _i < arguments.length; _i++) {
            args[_i] = arguments[_i];
        }
        this.v = [];
    }
    ev7_root.prototype.push = function (v) {
        this.v.push(v);
        return this;
    };
    ev7_root.prototype.dump = function () {
        var ret = JSON.stringify(this.v);
        this.v = [];
        return ret;
    };
    return ev7_root;
}());
function ev7() {
    var args = [];
    for (var _i = 0; _i < arguments.length; _i++) {
        args[_i] = arguments[_i];
    }
    return new ev7_root(args);
}
