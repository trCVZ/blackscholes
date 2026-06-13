<script>
    import { onMount } from "svelte";
    import { initWasm, add } from "./lib/wasm";

    let result = 0;

    onMount(async () => {
        await initWasm();
        result = add(2, 3);
    });
</script>

<h1>BlackScholes WASM test</h1>

<p>2 + 3 = {result}</p>