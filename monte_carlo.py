''' Steps:
    1: Select
    2: Expand
    3: Simulate/ Rollout
    4: Backpropagate
    '''

import math 
from Table import *
import random
import time
from copy import deepcopy 
QUANVALUE = 5
DISCOUNT_FACTOR = 0.01

table = [[0, 1], [5, 0], [5, 0], [5, 0], [5, 0], [5, 0], [0, 1], [5, 0], [5, 0], [5, 0], [5, 0], [5, 0]]
def get_coord(x, y):
    sol = (y - (HEIGHT/2 - 100)) // 100 * 5
    if sol == 0: #player 2's side
        return 11 - (x-200) // ((WIDTH - 400) / 5)
    else: #player 1's side
        return  (x-200) // ((WIDTH - 400) / 5) + 1

generated = dict()    
class game:
    def __init__(self, playerID,  table = table, player_point = [0, 0], N = 0, Q = 0, turn_step = 0):
        self.table = table
        self.turn_step = turn_step
        self.player_point = player_point
        self.playerID = playerID
        self.parent: game = None 
        self.children: list[game] = []
        # both N and Q will be updated in backpropagation method
        self.N = N
        self.Q = Q
        self.move = None
    
    
    def borrow_point(self, playerID):
        if playerID == 0:
            check = True if sum(self.table[1:6][0]) == 0 else False
            if check:
                self.player_point[0] -= 5
                for x in range(1, 6):
                    self.table[x][0] += 1
        
        if playerID == 1:
            check = True if sum(self.table[7:12][0]) == 0 else False
            if check:
                self.player_point[1] -= 5
                for x in range(1, 6):
                    self.table[x][0] += 1
                        

    def gen_valid_move(self, square: int, event = None, sign = None, playerID = 0):
        # print(f"Entering gen_valid_move with square={square}, event={event}, sign={sign}, playerID={self.playerID}, turn_step={self.turn_step}") # print for debug 
        if not self.game_over():
            self.borrow_point(playerID)
            if playerID == 0:
                if self.table[square][0] == 0 and square % 6 != 0:
                    if sign is None:
                        self.turn_step = 0
                        self.playerID = 1 - self.playerID
                        return False  
                    
                    square = (square + sign) % 12 
                    if self.table[square][0] > 0:
                        self.player_point[self.playerID] += self.table[square][0]
                        self.table[square] = [0, 0]
                        self.gen_valid_move(square = square, sign = sign)
                    
                    self.turn_step = 0
                    playerID = 1 

                if square % 6 == 0:
                    if sum(self.table[square]) > 0:
                        self.player_point[self.playerID] += (self.table[square][0] + QUANVALUE * self.table[square][1])
                        self.table[square] = [0, 0]
                    
                    self.turn_step = 0
                    self.playerID = 1 - self.playerID
                    return False
                
                if self.turn_step == 0:
                    if event == 'LEFT':
                        num_rocks = self.table[square][0]
                        self.table[square][0] = 0
                        while num_rocks > 0:
                            num_rocks -= 1
                            square = (square - 1) % 12
                            self.table[square][0] += 1
                        
                        self.turn_step += 1
                        sign = -1
                        if generated.get(tuple(map(tuple,self.table)), False) == False:
                            curr = game(playerID = self.playerID, table = self.table, player_point = self.player_point, turn_step = self.turn_step)
                            self.children.append(curr)
                            curr.parent = self
                            curr.move = event # save the move 
                            generated[tuple(map(tuple,self.table))] = True  
                            
                            curr.gen_valid_move(square = square, sign = sign) 
                        self.turn_step = 0
                    
                    if event == 'RIGHT':
                        num_rocks = self.table[square][0]
                        self.table[square][0] = 0
                        while num_rocks > 0:
                            num_rocks -= 1
                            square = (square + 1) % 12
                            self.table[square][0] += 1
                        
                        self.turn_step += 1
                        sign = 1
                        if generated.get(tuple(map(tuple,self.table)), False) == False:
                            curr = game(playerID = self.playerID, table = self.table, player_point = self.player_point, turn_step = self.turn_step)
                            self.children.append(curr)
                            curr.parent = self 
                            curr.move = event # save the move 
                            generated[tuple(map(tuple,self.table))] = True 
                            curr.gen_valid_move(square = square, sign = sign) 
                        
                        self.turn_step = 0
                    
                else:
                    num_rocks = self.table[square][0]
                    self.table[square][0] = 0
                    while num_rocks > 0:
                        num_rocks -= 1
                        square = (square + sign) % 12
                        self.table[square][0] += 1
                    
                    self.turn_step += 1
                    if generated.get(tuple(map(tuple,self.table)), False) == False:
                        curr = game(playerID = self.playerID, table = self.table, player_point = self.player_point, turn_step = self.turn_step)
                        self.children.append(curr)
                        curr.parent = self
                        generated[tuple(map(tuple,self.table))] = True 
                        curr.gen_valid_move(square = square, sign = sign)

            if playerID == 1:
                if event == 'LEFT':
                    event = 'RIGHT'
                if event == 'RIGHT':
                    event = 'LEFT'
                
                self.gen_valid_move(square = square, event = event)
        
        else: 
            self.playerID = 1 - self.playerID
            self.turn_step = 0
    
    def game_over(self):
        if (self.table[0] == [0, 0] and self.table[6] == [0, 0]):
            for i in range(12):
                self.table[i] = [0, 0] 
            return 1
        return 0
    def get_outcome(self):
        # if self.game_over():
        self.player_point[0] += (sum(self.table[1:6][0]) + QUANVALUE * sum(self.table[1:6][1]))
        self.player_point[1] += (sum(self.table[7:12][0]) + QUANVALUE * sum(self.table[7:12][1]))
        if self.player_point[self.playerID] > self.player_point[1 - self.playerID]:
            return 1
        
        elif self.player_point[self.playerID] == self.player_point[1 - self.playerID]:
            return 0
        
        else: return -1

    def add_nodes(self):
        ind = {0: [1, 2, 3, 4, 5], 1: [7, 8, 9, 10, 11]}
        if self.parent is None:
            for id in ind[self.playerID]:
                self.gen_valid_move(square = id, event = 'LEFT')
                self.gen_valid_move(square = id, event = 'RIGHT')

    def value(self, explore: int):
        if self.game_over(): return 0
        if self.N == 0:
            return 0 if explore == 0 else float('inf')
        
        return self.Q / self.N + explore * math.sqrt(math.log(self.parent.N) / self.N)

visited = dict() # dictionary used for marking visited game

class MCTS:
    def __init__(self, game: game, explore: int):
        self.game = game
        self.game.add_nodes()
        self.explore = explore 
        self.cumulative_reward = 0
        self.run_time = 0
        self.num_rollouts = 0

    def select_node(self, curr: game):
        curr.add_nodes()
        if len(curr.children) > 0:
            max_value = max(curr.children, key = lambda x: x.value(self.explore)).value(self.explore)
            max_games = [n for n in curr.children if n.value(self.explore) == max_value]
            next = random.choice(max_games)
            
            if visited.get(next, False) == False:
                visited[next] = True
                return next 

        return curr    

        # if self.expand(next):
        #     next = random.choice(next.children)
        
        # return next 

    def expand(self, curr: game):
        if not curr.game_over():
            curr.add_nodes()
            res = random.choice(curr.children)
            return res 

        else: return curr 

    def rollout(self, curr: game) -> int:
        while not curr.game_over():
            curr.add_nodes()
            max_value = max(curr.children, key = lambda x: x.value(self.explore)).value(self.explore)
            max_games = [n for n in curr.children if n.value(self.explore) == max_value]
            curr = random.choice(max_games)    
        return curr.get_outcome()

    def backpropagate(self, curr: game, outcome):
        discount_factor = DISCOUNT_FACTOR

        while curr is not None:
            curr.N += 1
            curr.Q += outcome
            # curr.value()
            self.cumulative_reward += outcome * discount_factor
            discount_factor *= DISCOUNT_FACTOR
            curr = curr.parent

    def search(self, time_limit: int):
        start_time = time.process_time()
        num_rollouts = 0
        curr = deepcopy(self.game)
        while time.process_time() - start_time < time_limit:
            # print(curr.table)
            curr = self.expand(curr)
            outcome = self.rollout(curr)    
            outcomes[outcome] += 1                              
            self.backpropagate(curr, outcome)
            num_rollouts += 1
            curr = self.select_node(curr)
        
        self.run_time = time.process_time() - start_time
        self.num_rollouts = num_rollouts

    def best_move(self):
        if self.game.game_over(): 
            return -1 
        
        max_value = max(self.game.children, key = lambda x: x.value(self.explore)).value(self.explore)
        max_games = [n for n in self.game.children if n.value(self.explore) == max_value]
        best_game = random.choice(max_games)
        return best_game.move 
    
    # def move(self):
    #     pass 

    def statistics(self):
        return self.num_rollouts, self.run_time

outcomes = {1: 0, -1: 0}
game_test = game(table = table, playerID = 1)
search = MCTS(game = game_test, explore = 0.5)
search.search(time_limit = 2)
for key in outcomes:
    print(f"{key} : {outcomes[key]}")
print(search.best_move())
print(search.statistics())