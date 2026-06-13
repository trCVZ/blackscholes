import createModule from "./wasm/blackscholes.js";

let module: any;

export async function initWasm() {
    module = await createModule();
}

export function add(a: number, b: number): number {
    return module.add(a, b);
}