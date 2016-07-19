import random

TYPE_INT = 1
TYPE_FLOAT = 2
TYPE_BOOL = 3

class variable:

    def __init__(self, name, type, min=None, max=None, sel=[]):
        self.name = name 
        self.type = type
        self.min = min
        self.max = max
        self.selection = sel

    def random(self):
        if self.type == TYPE_INT:
            if len(self.selection) > 0:
                i = random.randint(0,len(self.selection) - 1)
                return self.selection[i]
            else:    
                return random.randint(self.min, self.max)
        elif self.type == TYPE_FLOAT:
            if len(self.selection) > 0:
                i = random.randint(0,len(self.selection) - 1)
                return self.selection[i]
            else:    
                return random.uniform(self.min, self.max)
        elif self.type == TYPE_BOOL:
            a = random.randint(0, 1)
            if a == 0:
                return True
            else:
                return False        
        else:
            raise 'Unknown type'

class prototype:
    
    def __init__(self):
        self.variables = []
   
    def add_variable(self, v):
        self.variables.append(v)
 
    def random(self):
        d = {}
        for v in self.variables:
            d[v.name] = v.random()
        return d

    def random_bunch(self, n):
        d = []
        for i in xrange(n):
            d.append(self.random()) 
        return d

    def mate(self, a, b):
        d = {}
        for v in self.variables:
            z = random.randint(0, 1)
            if z == 0:
                d[v.name] = a[v.name]                
            else:
                d[v.name] = b[v.name]
        return d
    
    def mate2(self, a, b):
        z = random.randint(0, 1)
        if z == 0:
            try:
                del a['fitness']
            except:
                pass
            return a                  
        else:
            try:
                del b['fitness']
            except:
                pass
            return b

    def mutate(self, a, prob_mut = 0.1):
        z = random.uniform(0.0, 1.0)
        if z >= prob_mut:
            return a
        else:
            z = random.randint(0,len(self.variables)-1)
            a[self.variables[z].name] = self.variables[z].random()
            return a
    
    def mutate2(self, a, prob_mut = 0.1):
        z = random.uniform(0.0, 1.0)
        if z >= prob_mut:
            return a
        else:
            z = random.randint(0,len(self.variables)-1)
            var = a[self.variables[z].name]
            sel = self.variables[z].selection
            if len(sel) > 0:
                for i in xrange(len(sel)):
                    if sel[i] == var:
                        zz = random.randint(0, 1)
                        if zz == 0:
                            a[self.variables[z].name] = sel[(i - 1 + len(sel)) % len(sel)] 
                        else:
                            a[self.variables[z].name] = sel[(i + 1) % len(sel)] 
                        break
            else:
                a[self.variables[z].name] = self.variables[z].random()
            
            try:
                del a['fitness']
            except:
                pass
            return a

    def select_best(self, a, n):
        ff = []
        fitness_set = set()
        for i in xrange(len(a)):
            fit = a[i]['fitness']
            if fit in fitness_set:
                continue
            ff.append((fit, i))
            fitness_set.add(fit)
        ff = sorted(ff, reverse=True) 
        b = []
        for i in xrange(len(fitness_set)):
            b.append(a[ff[i][1]])
        return b

class evolver:

    def __init__(   self, 
                    proto, 
                    fitness_function,
                    population_size=20, 
                    selection_size=4,
                    init=None,
                    mutation_prob = 0.01,
                    fresh_blood_ratio = 0.1):
        self.proto = proto
        self.fitness_func = fitness_function
        self.pop_size = population_size
        self.sel_size = selection_size
        self.init = init
        self.mutation_prob = mutation_prob
        self.fresh_blood_ratio = fresh_blood_ratio
        self.population = self.start_population()

    def start_population(self):
        if self.init is None:
            return self.proto.random_bunch(self.pop_size)
        else:
            if(len(self.init) > self.pop_size):
                return init[:self.pop_size]
            else:
                a = self.proto.random_bunch(self.pop_size - len(self.init))
                for z in self.init:
                    a.append(z)
                return a

    def evaluate(self):
        self.population = self.fitness_func(self.population)

    def select_and_multiply(self):
        best = self.proto.select_best(self.population, self.sel_size)
        self.population = [] 
        i = 0
        fresh_blood_samples_count = int(self.fresh_blood_ratio * float(self.pop_size))

        while i < self.pop_size - fresh_blood_samples_count:
            k = random.randint(0, len(best) - 1)
            m = random.randint(0, len(best) - 1)
            if k == m:
                continue
            baby = self.proto.mate2(best[k], best[m])
            baby = self.proto.mutate2(baby,prob_mut=self.mutation_prob)
            self.population.append(baby)
            i += 1
        
        for i in xrange(fresh_blood_samples_count):
            self.population.append(self.proto.random())
                    
    def run_n_generations(self, n):
        for i in xrange(n):
            print 'running generation %d' % i
            self.evaluate()
            self.select_and_multiply()
        self.evaluate()



