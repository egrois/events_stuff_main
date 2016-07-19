import numpy as np

def bond_price(face_value, coupon_rate, years, times_per_year, yield_rate):
    rate = coupon_rate / float( times_per_year )
    cash = float( face_value ) * rate
    periods = years * times_per_year
    yield_rate2 = yield_rate / float( times_per_year ) 
    yy = 1.0 / np.power(1.0 + yield_rate2, int(periods))
    return cash * ((1.0 - yy) / yield_rate2) + float(face_value) * yy 

def yield_to_maturity(face_value, price, coupon_rate, years, times_per_year = 1, precision = 0.000000001):    
    y1 = 0.0
    y2 = 1.0
    
    while(np.abs(y2 - y1) > precision):
        y = (y1 + y2) * 0.5
        p = bond_price(face_value, coupon_rate, years, times_per_year, y)
        
        if p > price:
            y1 = y
        else:
            y2 = y            
    return y

def estimate_dollar_beta(r1, r2, d1, d2):
    dr1 = bond_price(100000.0, 0.06, d1, 2, r1) - bond_price(100000.0, 0.06, d1, 2, r1 + 0.001)
    dr2 = bond_price(100000.0, 0.06, d2, 2, r2) - bond_price(100000.0, 0.06, d2, 2, r2 + 0.001)
    return dr1 / dr2

def estimate_dollar_beta2(p1, p2, d1, d2):
    r1 = yield_to_maturity(100000.0, p1, 0.06, d1, times_per_year=2)
    r2 = yield_to_maturity(100000.0, p2, 0.06, d2, times_per_year=2)
    return estimate_dollar_beta(r1,r2,d1,d2)
