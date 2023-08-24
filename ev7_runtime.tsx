class ev7_root {
    private v: any[]

    constructor(...args) {
        this.v = []
    }

    push(v: any) {
        this.v.push(v)
    
        return this
    }

    dump() {
        var ret = JSON.stringify(this.v)

        this.v = []
    
        return ret
    }
}


function ev7(...args) {
    return new ev7_root(args)
}
