#include <emscripten/bind.h>
#include <emscripten/val.h>
#include <cmath>
#include <vector>

using namespace emscripten;

namespace {

    double norm_cdf(double x) {
        return 0.5 * std::erfc(-x / std::sqrt(2.0));
    }

    double norm_pdf(double x) {
        static const double inv_sqrt_2pi = 1.0 / std::sqrt(2.0 * M_PI);
        return inv_sqrt_2pi * std::exp(-0.5 * x * x);
    }

    struct PricingResult {
        double call_price = 0.0;
        double put_price  = 0.0;
        double call_delta = 0.0;
        double put_delta  = 0.0;
        double call_gamma = 0.0;
        double put_gamma  = 0.0;
    };

    PricingResult price(double time_to_maturity, double strike, double current_price,
                        double volatility, double interest_rate) {
        PricingResult res;

        double        d1 = (std::log(current_price / strike) +
                     (interest_rate + 0.5 * volatility * volatility) * time_to_maturity) /
            (volatility * std::sqrt(time_to_maturity));
        double d2 = d1 - volatility * std::sqrt(time_to_maturity);

        res.call_price = current_price * norm_cdf(d1) -
            strike * std::exp(-interest_rate * time_to_maturity) * norm_cdf(d2);

        res.put_price = strike * std::exp(-interest_rate * time_to_maturity) * norm_cdf(-d2) -
            current_price * norm_cdf(-d1);

        res.call_delta = norm_cdf(d1);
        res.put_delta  = res.call_delta - 1.0;

        res.call_gamma = norm_pdf(d1) / (strike * volatility * std::sqrt(time_to_maturity));
        res.put_gamma  = res.call_gamma;

        return res;
    }

}

class BlackScholes {
  public:
    BlackScholes(double time_to_maturity, double strike, double current_price, double volatility,
                 double interest_rate) :
        time_to_maturity_(time_to_maturity), strike_(strike), current_price_(current_price),
        volatility_(volatility), interest_rate_(interest_rate) {}

    void calculate_prices() {
        result_ = price(time_to_maturity_, strike_, current_price_, volatility_, interest_rate_);
    }

    double get_call_price() const {
        return result_.call_price;
    }
    double get_put_price() const {
        return result_.put_price;
    }
    double get_call_delta() const {
        return result_.call_delta;
    }
    double get_put_delta() const {
        return result_.put_delta;
    }
    double get_call_gamma() const {
        return result_.call_gamma;
    }
    double get_put_gamma() const {
        return result_.put_gamma;
    }

  private:
    double        time_to_maturity_;
    double        strike_;
    double        current_price_;
    double        volatility_;
    double        interest_rate_;
    PricingResult result_;
};

emscripten::val compute_heatmap(double time_to_maturity, double strike, double interest_rate,
                                double spot_min, double spot_max, double vol_min, double vol_max,
                                int steps) {
    if (steps < 2)
        steps = 2;

    std::vector<double> spot_range(steps);
    std::vector<double> vol_range(steps);
    for (int i = 0; i < steps; ++i) {
        double t      = static_cast<double>(i) / (steps - 1);
        spot_range[i] = spot_min + (spot_max - spot_min) * t;
        vol_range[i]  = vol_min + (vol_max - vol_min) * t;
    }

    std::vector<double> call_grid(steps * steps);
    std::vector<double> put_grid(steps * steps);

    for (int i = 0; i < steps; ++i) {
        for (int j = 0; j < steps; ++j) {
            PricingResult res =
                price(time_to_maturity, strike, spot_range[j], vol_range[i], interest_rate);
            call_grid[i * steps + j] = res.call_price;
            put_grid[i * steps + j]  = res.put_price;
        }
    }

    emscripten::val out = emscripten::val::object();
    out.set("spotRange", emscripten::val::array(spot_range));
    out.set("volRange", emscripten::val::array(vol_range));
    out.set("callGrid", emscripten::val::array(call_grid));
    out.set("putGrid", emscripten::val::array(put_grid));
    return out;
}

EMSCRIPTEN_BINDINGS(black_scholes_module) {
    class_<BlackScholes>("BlackScholes")
        .constructor<double, double, double, double, double>()
        .function("calculate_prices", &BlackScholes::calculate_prices)
        .function("get_call_price", &BlackScholes::get_call_price)
        .function("get_put_price", &BlackScholes::get_put_price)
        .function("get_call_delta", &BlackScholes::get_call_delta)
        .function("get_put_delta", &BlackScholes::get_put_delta)
        .function("get_call_gamma", &BlackScholes::get_call_gamma)
        .function("get_put_gamma", &BlackScholes::get_put_gamma);

    function("compute_heatmap", &compute_heatmap);
}
