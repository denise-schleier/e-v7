
export class ev7_root {
    private argv: string[]
    private v: any[]

    constructor(argv) {
        this.argv = argv
        this.v = []
    }

    load(name: string) {
        return require(name)
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
