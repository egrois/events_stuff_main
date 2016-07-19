import numpy as np

linear = 1
quadratic = 2
constant = 3

LAST_TRADE_BID = 1
LAST_TRADE_ASK = 2
LAST_TRADE_NONE = 3


class stacker:

    def __init__(self):
        self.mpi = 25 
        self.num_levels = 500
        self.skip_mask = 1

        self.ask_replenish_edge = 1
        self.ask_replenish_size = 1
        self.ask_replenish = True
        self.ask_ut_type = linear
        self.ask_ut_param_1 = 0.01
        self.ask_ut_param_2 = 1.0

        self.bid_replenish_edge = 1
        self.bid_replenish_size = 1
        self.bid_replenish = True
        self.bid_ut_type = linear
        self.bid_ut_param_1 = 0.01
        self.bid_ut_param_2 = 1.0
    
        self.bid_side = {}
        self.ask_side = {}
        
        self.ref = 0
        self.bid_ref = 0
        self.ask_ref = 0
        self.last_trade = LAST_TRADE_NONE
        self.last_traded_bid_price = 0
        self.last_traded_ask_price = 0
        
    def bid_size(self, edge):
        if self.bid_ut_type == constant:
            return self.bid_ut_param_1
        elif self.bid_ut_type == linear:
            return int(self.bid_ut_param_1 * float(edge) + float(self.bid_ut_param_2)) 
        elif self.bid_ut_type == quadratic:
            return int(self.bid_ut_param_1 * float(edge*edge) + float(self.bid_ut_param_2))
        
    def ask_size(self, edge):
        if self.ask_ut_type == constant:
            return self.ask_ut_param_1
        elif self.ask_ut_type == linear:
            return int(self.ask_ut_param_1 * float(edge) + float(self.ask_ut_param_2)) 
        elif self.ask_ut_type == quadratic:
            return int(self.ask_ut_param_1 * float(edge*edge) + float(self.ask_ut_param_2))

    def top_bid_price(self):
        return sorted(self.bid_side)[-1]
    
    def top_ask_price(self):
        return sorted(self.ask_side)[0]  
    
    def top_bid_size(self):
        return self.bid_side[self.top_bid_price()]
    
    def top_ask_size(self):
        return self.ask_side[self.top_ask_price()]       
    
    def update(self, bid, ask):
        
        trades = []
        
        bids_to_remove = []
        asks_to_remove = []
        
        if len(self.bid_side) > 0 and bid < self.top_bid_price():
            for price in sorted(self.bid_side, reverse=True):
                if price <= bid:
                    break
                size = self.bid_side[price]
                tt = (price, size)
                trades.append(tt)
                self.last_trade = LAST_TRADE_BID 
                self.last_traded_bid_price = price 
                bids_to_remove.append(price)
        
        if len(self.ask_side) > 0 and ask > self.top_ask_price():
            for price in sorted(self.ask_side):
                if price >= ask:
                    break
                size = self.ask_side[price]
                tt = (price, -size)
                trades.append(tt)
                self.last_trade = LAST_TRADE_ASK 
                self.last_traded_ask_price = price
                asks_to_remove.append(price)
                
        for br in bids_to_remove:
            del self.bid_side[br]
            
        for ar in asks_to_remove:
            del self.ask_side[ar] 
    
        if len(self.ask_side) == 0:
            print 'ASK_SIDE exhausted'
            raise
        
        if len(self.bid_side) == 0:
            print 'BID_SIDE exhausted'
            raise
   
        if self.ask_replenish == True and len(self.ask_side) > 0 and self.last_trade != LAST_TRADE_ASK:
            price = self.top_ask_price() - self.mpi
            while price >= ask + self.ask_replenish_edge:
                i = ((price - self.ask_ref) / self.mpi)
                if i % self.skip_mask == 0:
                    if self.last_traded_bid_price == 0 or self.last_traded_bid_price < price:
                        self.ask_side[price] = self.ask_replenish_size
                price -= self.mpi
        
        if self.bid_replenish == True and len(self.bid_side) > 0 and self.last_trade != LAST_TRADE_BID:
            price = self.top_bid_price() + self.mpi
            while price <= bid - self.bid_replenish_edge:
                i = ((self.bid_ref - price) / self.mpi)
                if i % self.skip_mask == 0:
                    if self.last_traded_ask_price == 0 or self.last_traded_ask_price > price:
                        self.bid_side[price] = self.bid_replenish_size
                price += self.mpi
        
        return trades
    
    def reset(self, bid, ask):
        self.bid_side = {}
        self.ask_side = {}
        self.ref = float(ask+bid) * 0.5
        self.bid_ref = bid
        self.ask_ref = ask
        
        for i in xrange(self.num_levels):
            if i % self.skip_mask != 0:
                continue
            b_size = self.bid_size(i)
            b_price = self.bid_ref - (i + self.bid_replenish_edge) * self.mpi
            if b_size > 0:
                self.bid_side[b_price] = b_size
            
            a_size = self.ask_size(i)
            a_price = self.ask_ref + (i + self.ask_replenish_edge) * self.mpi
            if a_size > 0:
                self.ask_side[a_price] = a_size
        self.last_trade = LAST_TRADE_NONE
        self.last_traded_bid_price = 0
        self.last_traded_ask_price = 0

def init_game(game, start_time):
    game['pnl'] = 0.0
    game['pos'] = 0.0
    game['vol'] = 0

    game['bid_start'] = 0
    game['ask_start'] = 0

    game['bid_stop'] = 0
    game['ask_stop'] = 0

    game['start_time'] = start_time
    game['stop_time'] = 0

    game['is_finished'] = False
    game['pnl_per_trade'] = 0.0

    game['num_trades'] = 0
    
    return game

def update_game(game, time, bid, ask, pnl, pos, vol):

    if game['bid_start'] == 0:
        game['bid_start'] = bid
        game['ask_start'] = ask
    
    game['bid_stop'] = bid
    game['ask_stop'] = ask
    
    game['pnl'] = pnl
    game['vol'] += vol
    if(vol > 0):
        game['num_trades'] += 1

    game['stop_time'] = time

    alpha = float(pos) * float(game['pos'])

    if game['num_trades'] >= 2 and alpha <= 0.1:
        game['is_finished'] = True
        game['pnl_per_trade'] = game['pnl'] / game['vol'] 
    
    game['pos'] = pos

def run_bt( bid = None,
            ask = None,
            sigma = None, #
            use_r2 = False,
            r2_threshold = 1.1,
            mpi = None,
            mpi_value = None,
            st = None,
            pnl_target = 12,
            pnl_target_thres = 10,
            use_dynamic_pnl_target = True,
            dynamic_pnl_target_slope = 0.001,
            r2_pnl_target = 12,
            r2_pnl_target_thres = 10,
            r2_use_dynamic_pnl_target = True,
            r2_dynamic_pnl_target_slope = 0.001,
            max_time_to_start_game = 20800,
            max_drawdown = -10000.0,
            game_drawdown = -1000,
            r2_game_drawdown = -1000,
            max_prize = 7000.0,
            dynamic_replenish_coef = 1.0,
            use_dynamic_replenish_size = False):

    num = len(bid)
    to_dol = mpi_value / float(mpi)
    st.reset(bid[0], ask[0])
    pos = np.zeros(num)
    cash = np.zeros(num)
    pnl = np.zeros(num)
    games = [] 
    game = {}
    all_trades = []
    closed_pnl = 0.0
    trading_enabled = True
    min_replenish = st.ask_replenish_size

    rv_pnl_target = None
    rv_pnl_target_thres = None
    rv_use_dynamic_pnl_target = None
    rv_dynamic_pnl_target_slope = None
    rv_game_drawdown = None

    for i in xrange(num):
        if i == 0:
           
            if sigma[0] > r2_threshold and use_r2:
                rv_pnl_target = r2_pnl_target
                rv_pnl_target_thres = r2_pnl_target_thres
                rv_use_dynamic_pnl_target = r2_use_dynamic_pnl_target
                rv_dynamic_pnl_target_slope = r2_dynamic_pnl_target_slope
                rv_game_drawdown = r2_game_drawdown
            else:
                rv_pnl_target = pnl_target
                rv_pnl_target_thres = pnl_target_thres
                rv_use_dynamic_pnl_target = use_dynamic_pnl_target
                rv_dynamic_pnl_target_slope = dynamic_pnl_target_slope
                rv_game_drawdown = game_drawdown
            
            game = init_game(game, i)
            continue

        if trading_enabled == False:
            pos[i] = pos[i-1]
            pnl[i] = pnl[i-1]
            cash[i] = cash[i-1]
            continue

        trades = st.update(bid[i], ask[i])
 
        pos[i] = pos[i-1]
        cash[i] = cash[i-1]

        vol = 0
        if len(trades) > 0:
            for trade in trades:
                all_trades.append((i, trade[0], trade[1]))
                vol += np.abs(trade[1])
                pos[i] = pos[i] + float(trade[1])
                cash[i] = cash[i]  - float(trade[1]) * to_dol * float(trade[0])
        else:
            abs_pos = np.abs(game['pos'])

            if game['pnl'] < rv_game_drawdown:
                trade_price = bid[i] if game['pos'] > 0 else ask[i]
                trade_size = -abs_pos if game['pos'] > 0 else abs_pos
                all_trades.append((i, trade_price, trade_size))
                vol += abs_pos
                pos[i] = 0
                cash[i] = cash[i]  - float(trade_price) * to_dol * float(trade_size)

            elif pnl[i-1] > max_prize or pnl[i-1] < max_drawdown:
                
                #if pnl[i-1] > max_prize:
                    #print 'PRIZE'
                #else:
                    #print 'DRAWDOWN'

                trading_enabled = False
                trade_price = bid[i] if game['pos'] > 0 else ask[i]
                trade_size = -abs_pos if game['pos'] > 0 else abs_pos
                all_trades.append((i, trade_price, trade_size))
                vol += abs_pos
                pos[i] = 0
                cash[i] = cash[i]  - float(trade_price) * to_dol * float(trade_size)
            elif abs_pos > rv_pnl_target_thres:
                target = rv_pnl_target
                if rv_use_dynamic_pnl_target == True:
                    target = rv_pnl_target - float(game['stop_time'] - game['start_time']) * rv_dynamic_pnl_target_slope                 
                if (game['pnl'] / (game['vol'] + abs_pos)) > target:
                    trade_price = bid[i] if game['pos'] > 0 else ask[i]
                    trade_size = -abs_pos if game['pos'] > 0 else abs_pos
                    all_trades.append((i, trade_price, trade_size))
                    vol += abs_pos
                    pos[i] = 0
                    cash[i] = cash[i]  - float(trade_price) * to_dol * float(trade_size)
        
        if pos[i] >= 0:
            pnl[i] = cash[i] + float(bid[i]) * pos[i] * to_dol
        else:
            pnl[i] = cash[i] + float(ask[i]) * pos[i] * to_dol

        if use_dynamic_replenish_size == True:
            st.bid_replenish_size = max(min_replenish, int(dynamic_replenish_coef * np.sqrt(np.abs(pos[i]))))
            st.ask_replenish_size = max(min_replenish, int(dynamic_replenish_coef * np.sqrt(np.abs(pos[i]))))

        update_game(game, i, bid[i], ask[i], pnl[i]-closed_pnl, pos[i], vol )

        if game['is_finished'] == True:
            games.append(game)
            if i > max_time_to_start_game:
                #print 'LAST_GAME'
                trading_enabled = False
                continue            
            game = {}
            if sigma[i] > r2_threshold and use_r2:
                rv_pnl_target = r2_pnl_target
                rv_pnl_target_thres = r2_pnl_target_thres
                rv_use_dynamic_pnl_target = r2_use_dynamic_pnl_target
                rv_dynamic_pnl_target_slope = r2_dynamic_pnl_target_slope
                rv_game_drawdown = r2_game_drawdown
            else:
                rv_pnl_target = pnl_target
                rv_pnl_target_thres = pnl_target_thres
                rv_use_dynamic_pnl_target = use_dynamic_pnl_target
                rv_dynamic_pnl_target_slope = dynamic_pnl_target_slope
                rv_game_drawdown = game_drawdown
            init_game(game, i)
            st.reset(bid[i], ask[i])
            closed_pnl = pnl[i]

    if game['is_finished'] == False:
        if game['pos'] > 0:
            all_trades.append((num - 1,bid[-1] , -game['pos'] ))
        else:
            all_trades.append((num - 1, ask[-1] , -game['pos'] ))
        update_game(game, num-1, bid[-1], ask[-1], pnl[-1], 0, vol + np.abs(game['pos']))
        games.append(game)

    return(pnl,pos,cash, games, all_trades)



def multiday_bt(stack, 
                data ,
                pnl_target,
                pnl_target_thres,
                use_dynamic_pnl_target,
                dynamic_pnl_target_slope,
                r2_pnl_target,
                r2_pnl_target_thres,
                r2_use_dynamic_pnl_target,
                r2_dynamic_pnl_target_slope,
                max_time_to_start_game,
                max_drawdown, 
                max_prize,
                dynamic_replenish_coef,
                use_dynamic_replenish_size,
                game_drawdown,
                r2_game_drawdown,
                r2_threshold,
                use_r2 
                ):
    results = {}
    for dt in sorted(data.keys()):
        results[dt] = run_bt(   data[dt]['bid'], 
                                data[dt]['ask'], 
                                data[dt]['vol'],
                                mpi = 25, 
                                mpi_value = 12.5,
                                st = stack, 
                                pnl_target=pnl_target,
                                pnl_target_thres=pnl_target_thres,
                                use_dynamic_pnl_target = use_dynamic_pnl_target,
                                dynamic_pnl_target_slope=dynamic_pnl_target_slope,
                                r2_pnl_target = r2_pnl_target,
                                r2_pnl_target_thres = r2_pnl_target_thres,
                                r2_use_dynamic_pnl_target = r2_use_dynamic_pnl_target,
                                r2_dynamic_pnl_target_slope = r2_dynamic_pnl_target_slope,
                                max_time_to_start_game = max_time_to_start_game,
                                max_drawdown = max_drawdown,
                                max_prize = max_prize,
                                dynamic_replenish_coef = dynamic_replenish_coef,
                                use_dynamic_replenish_size = use_dynamic_replenish_size,
                                game_drawdown = game_drawdown,
                                r2_game_drawdown = r2_game_drawdown,
                                r2_threshold = r2_threshold,
                                use_r2=use_r2
                                )
    return results
