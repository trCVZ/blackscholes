<script>
  export let title = '';
  export let spotRange = [];
  export let volRange = []; 
  export let grid = [];
  export let xLabel = 'Spot Price';
  export let yLabel = 'Volatility';

  const VIRIDIS_STOPS = [
    [0.267, 0.005, 0.329],
    [0.283, 0.141, 0.458],
    [0.254, 0.265, 0.530],
    [0.207, 0.372, 0.553],
    [0.164, 0.471, 0.558],
    [0.128, 0.567, 0.551],
    [0.135, 0.659, 0.518],
    [0.267, 0.749, 0.441],
    [0.478, 0.821, 0.318],
    [0.741, 0.873, 0.150],
    [0.993, 0.906, 0.144],
  ];

  function viridis(t) {
    const clamped = Math.min(1, Math.max(0, t));
    const n = VIRIDIS_STOPS.length - 1;
    const pos = clamped * n;
    const idx = Math.min(n - 1, Math.floor(pos));
    const frac = pos - idx;
    const [r1, g1, b1] = VIRIDIS_STOPS[idx];
    const [r2, g2, b2] = VIRIDIS_STOPS[idx + 1];
    const r = Math.round((r1 + (r2 - r1) * frac) * 255);
    const g = Math.round((g1 + (g2 - g1) * frac) * 255);
    const b = Math.round((b1 + (b2 - b1) * frac) * 255);
    return `rgb(${r}, ${g}, ${b})`;
  }

  function textColorFor(t) {
    return t > 0.6 ? '#10151c' : '#f5f5f5';
  }

  $: n = spotRange.length;
  $: minVal = grid.length ? Math.min(...grid) : 0;
  $: maxVal = grid.length ? Math.max(...grid) : 1;
  $: span = maxVal - minVal || 1;
</script>

<div class="heatmap-wrap">
  {#if title}<h4 class="heatmap-title">{title}</h4>{/if}

  <div class="heatmap-body">
    <div class="y-axis-label">{yLabel}</div>

    <div
      class="grid"
      style="grid-template-columns: 64px repeat({n}, 1fr); grid-template-rows: repeat({n}, 1fr) 28px;"
    >
      {#each volRange as v, i}
        <div class="row-label">{v.toFixed(2)}</div>
        {#each spotRange as s, j}
          {@const value = grid[i * n + j]}
          {@const t = (value - minVal) / span}
          <div class="cell" style="background-color: {viridis(t)}; color: {textColorFor(t)};">
            {value.toFixed(2)}
          </div>
        {/each}
      {/each}

      <div class="corner"></div>
      {#each spotRange as s}
        <div class="col-label">{s.toFixed(2)}</div>
      {/each}
    </div>
  </div>

  <div class="x-axis-label">{xLabel}</div>
</div>

<style>
  .heatmap-wrap {
    color: var(--text-color, #fafafa);
    width: 100%;
  }

  .heatmap-title {
    text-align: center;
    margin: 0 0 10px 0;
    font-weight: 600;
  }

  .heatmap-body {
    display: flex;
    align-items: center;
    gap: 6px;
  }

  .y-axis-label {
    writing-mode: vertical-rl;
    transform: rotate(180deg);
    font-size: 0.8rem;
    opacity: 0.85;
    white-space: nowrap;
  }

  .grid {
    display: grid;
    gap: 2px;
    flex: 1;
  }

  .cell {
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 0.68rem;
    font-weight: 600;
    min-height: 34px;
    border-radius: 2px;
  }

  .row-label,
  .col-label,
  .corner {
    display: flex;
    align-items: center;
    justify-content: center;
    font-size: 0.68rem;
    opacity: 0.85;
  }

  .x-axis-label {
    text-align: center;
    margin-top: 4px;
    font-size: 0.8rem;
    opacity: 0.85;
  }
</style>