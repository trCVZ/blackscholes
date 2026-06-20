let modulePromise = null;


export function loadBlackScholes() {
  if (!modulePromise) {
    modulePromise = import('../wasm/blackscholes.js').then((mod) =>
      mod.default()
    );
  }
  return modulePromise;
}


export function pricePoint(Module, { timeToMaturity, strike, currentPrice, volatility, interestRate }) {
  const bs = new Module.BlackScholes(timeToMaturity, strike, currentPrice, volatility, interestRate);
  bs.calculate_prices();
  const result = {
    callPrice: bs.get_call_price(),
    putPrice: bs.get_put_price(),
    callDelta: bs.get_call_delta(),
    putDelta: bs.get_put_delta(),
    callGamma: bs.get_call_gamma(),
    putGamma: bs.get_put_gamma(),
  };
  bs.delete(); 
  return result;
}

export function computeHeatmap(Module, { timeToMaturity, strike, interestRate, spotMin, spotMax, volMin, volMax, steps = 10 }) {
  const raw = Module.compute_heatmap(timeToMaturity, strike, interestRate, spotMin, spotMax, volMin, volMax, steps);
  return {
    spotRange: Array.from(raw.spotRange),
    volRange: Array.from(raw.volRange),
    callGrid: Array.from(raw.callGrid),
    putGrid: Array.from(raw.putGrid),
  };
}