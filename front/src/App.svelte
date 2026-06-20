<script>
  import { onMount } from 'svelte';
  import { loadBlackScholes, pricePoint, computeHeatmap } from './lib/blackscholes.js';
  import ValueCard from './lib/ValueCard.svelte';
  import Heatmap from './lib/Heatmap.svelte';
  import './app.css';

  let Module = null;
  let ready = false;

  let currentPrice = 100.0;
  let strike = 100.0;
  let timeToMaturity = 1.0;
  let volatility = 0.2;
  let interestRate = 0.05;

  let spotMin = currentPrice * 0.8;
  let spotMax = currentPrice * 1.2;
  let volMin = volatility * 0.5;
  let volMax = volatility * 1.5;

  let callPrice = 0;
  let putPrice = 0;
  let heatmap = null;

  onMount(async () => {
    Module = await loadBlackScholes();
    ready = true;
    recompute();
  });

  function recompute() {
    if (!ready || !Module) return;

    const point = pricePoint(Module, {
      timeToMaturity,
      strike,
      currentPrice,
      volatility,
      interestRate,
    });
    callPrice = point.callPrice;
    putPrice = point.putPrice;

    heatmap = computeHeatmap(Module, {
      timeToMaturity,
      strike,
      interestRate,
      spotMin,
      spotMax,
      volMin,
      volMax,
      steps: 10,
    });
  }

  $: {
    currentPrice;
    strike;
    timeToMaturity;
    volatility;
    interestRate;
    spotMin;
    spotMax;
    volMin;
    volMax;
    if (ready) recompute();
  }
</script>

<div class="layout">
  <aside class="sidebar">
    <h2>Black-Scholes Model</h2>

    <hr />

    <label>
      Current Asset Price
      <input type="number" step="0.01" bind:value={currentPrice} />
    </label>

    <label>
      Strike Price
      <input type="number" step="0.01" bind:value={strike} />
    </label>

    <label>
      Time to Maturity (Years)
      <input type="number" step="0.01" bind:value={timeToMaturity} />
    </label>

    <label>
      Volatility
      <input type="number" step="0.01" bind:value={volatility} />
    </label>

    <label>
      Risk-Free Interest Rate
      <input type="number" step="0.01" bind:value={interestRate} />
    </label>

    <hr />

    <h3>Heatmap Parameters</h3>

    <label>
      Min Spot Price
      <input type="number" step="0.01" min="0.01" bind:value={spotMin} />
    </label>

    <label>
      Max Spot Price
      <input type="number" step="0.01" min="0.01" bind:value={spotMax} />
    </label>

    <label>
      Min Volatility for Heatmap
      <input type="range" min="0.01" max="1" step="0.01" bind:value={volMin} />
      <span class="slider-value">{volMin.toFixed(2)}</span>
    </label>

    <label>
      Max Volatility for Heatmap
      <input type="range" min="0.01" max="1" step="0.01" bind:value={volMax} />
      <span class="slider-value">{volMax.toFixed(2)}</span>
    </label>
  </aside>

  <main class="content">
    <h1>Black-Scholes Pricing Model</h1>

    <table>
      <thead>
        <tr>
          <th>Current Asset Price</th>
          <th>Strike Price</th>
          <th>Time to Maturity (Years)</th>
          <th>Volatility (σ)</th>
          <th>Risk-Free Interest Rate</th>
        </tr>
      </thead>
      <tbody>
        <tr>
          <td>{currentPrice}</td>
          <td>{strike}</td>
          <td>{timeToMaturity}</td>
          <td>{volatility}</td>
          <td>{interestRate}</td>
        </tr>
      </tbody>
    </table>

    <div class="values-row">
      <ValueCard label="CALL Value" value={callPrice} variant="call" />
      <ValueCard label="PUT Value" value={putPrice} variant="put" />
    </div>

    <h1>Options Price - Interactive Heatmap</h1>
    <div class="info-box">
      Explore comment les prix des options varient selon le "Spot Price" et la
      volatilité, à "Strike Price" constant.
    </div>

    {#if heatmap}
      <div class="heatmaps-row">
        <div class="heatmap-col">
          <h3>Call Price Heatmap</h3>
          <Heatmap
            title=""
            spotRange={heatmap.spotRange}
            volRange={heatmap.volRange}
            grid={heatmap.callGrid}
          />
        </div>
        <div class="heatmap-col">
          <h3>Put Price Heatmap</h3>
          <Heatmap
            title=""
            spotRange={heatmap.spotRange}
            volRange={heatmap.volRange}
            grid={heatmap.putGrid}
          />
        </div>
      </div>
    {:else}
      <p>Chargement du moteur de calcul (wasm)…</p>
    {/if}
  </main>
</div>

<style>
  .layout {
    display: flex;
    min-height: 100vh;
  }

  .sidebar {
    width: 300px;
    flex-shrink: 0;
    background-color: var(--secondary-background-color);
    padding: 20px;
    box-sizing: border-box;
  }

  .sidebar h2 {
    margin-top: 0;
    font-size: 1.2rem;
  }

  .sidebar hr {
    border: none;
    border-top: 1px solid rgba(255, 255, 255, 0.15);
    margin: 16px 0;
  }

  .sidebar label {
    display: block;
    font-size: 0.85rem;
    margin-bottom: 14px;
  }

  .sidebar input[type='number'] {
    margin-top: 4px;
    background-color: var(--background-color);
    color: var(--text-color);
    border: 1px solid rgba(255, 255, 255, 0.2);
    border-radius: 6px;
    padding: 6px 8px;
  }

  .sidebar input[type='range'] {
    margin-top: 6px;
    accent-color: var(--primary-color);
  }

  .slider-value {
    font-size: 0.75rem;
    opacity: 0.75;
  }

  .content {
    flex: 1;
    padding: 24px 32px;
    max-width: 1200px;
  }

  .content h1 {
    font-size: 1.8rem;
  }

  .values-row {
    display: flex;
    gap: 16px;
    margin: 20px 0 32px 0;
  }

  .values-row > :global(div) {
    flex: 1;
  }

  .heatmaps-row {
    display: flex;
    gap: 24px;
    margin-top: 16px;
  }

  .heatmap-col {
    flex: 1;
    min-width: 0;
  }

  .heatmap-col h3 {
    text-align: center;
    font-size: 1rem;
  }
</style>