<script>

  export let label = '';
  export let value = 0;
  export let variant = 'call';
</script>

<div class="metric-container metric-{variant}">
  <div>
    <div class="metric-label">{label}</div>
    <div class="metric-value">${value.toFixed(2)}</div>
  </div>
</div>

<style>
  .metric-container {
    display: flex;
    justify-content: center;
    align-items: center;
    padding: 14px;
    border-radius: 10px;
    width: 100%;
    box-sizing: border-box;
  }

  .metric-call {
    background-color: #90ee90;
  }

  .metric-put {
    background-color: #ffcccb;
  }

  .metric-label {
    font-size: 1rem;
    margin-bottom: 4px;
    color: #000;
  }

  .metric-value {
    font-size: 1.6rem;
    font-weight: bold;
    margin: 0;
    color: #000;
  }
</style>