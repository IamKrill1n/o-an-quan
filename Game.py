from subprocess import Popen, PIPE
import random 
from Table import *
def get_coord(x, y):
    sol = (y - (HEIGHT/2 - 100)) // 100 * 5
    if sol == 0: #player 2's side
        return 11 - (x-200) // ((WIDTH - 400) / 5)
    else: #player 1's side
        return  (x-200) // ((WIDTH - 400) / 5) + 1

def getRandom(playerID, table):
    move = []
    if(playerID == 0):
        for i in range(1, 6):
            if table[i][0] > 0:
                move.append([i, 0])
                move.append([i, 1])
    else:
        for i in range(1, 6):
            if table[i][0] > 0:
                move.append([i, 0])
                move.append([i, 1])
    return random.choice(move)

class Game:
    def __init__(self):
        self.state = Table()
        self.turn_step = 0
        self.AI_move = None

    def redraw(self):
        self.state.redraw(self.turn_step)

    
    def check_valid_move(self, event):
        x_coord = event.pos[0]
        y_coord = event.pos[1]
        
        cur = int(get_coord(x_coord, y_coord))
        if 0 < cur < 12 and self.state.table[cur] != [0,0]:
            if (self.turn_step <= 1 and 0 < cur < 6) or (self.turn_step > 1 and 6 < cur < 12):
                self.state.selection, self.state.picking = cur, cur
                self.turn_step = 1 if self.turn_step <= 1 else 3

    def play(self, cell, event): #find direction and play
        if self.turn_step == 1 or self.turn_step == 3:
            playerID = self.turn_step//2
            #1 is ccw, 0 is cw
            #for player 1, going left is cw, right is ccw
            #for player 2, going left is ccw, right is cw   
            direction = (playerID + (event == pygame.K_RIGHT)) % 2
            self.state.update(playerID, cell, direction, self.turn_step)
            self.turn_step = (self.turn_step + 1) % 4
        
    def fix_empty_rows(self):
        if self.turn_step == 0 and self.state.empty_rows[0]:
            self.state.borrow_point(0, self.turn_step)
        if self.turn_step == 2 and self.state.empty_rows[1]:
            self.state.borrow_point(1, self.turn_step)
    
    def game_over(self):
        if self.state.table[0] == [0,0] and self.state.table[6] == [0,0]:
            self.state.player_point[0] += sum(self.state.table[i][0] for i in range(1, 6))
            self.state.player_point[1] += sum(self.state.table[i][0] for i in range(7, 12))
            for i in range(12):
                self.state.table[i] = [0, 0]
            return True
        elif self.state.player_point[0] < -50 or self.state.player_point[1] < -50:
            return True
        return False

    def make_AI_move(self, strategy, program_path = "main.exe", depth = 7, utility = 1): #print cell and direction
        playerID = self.turn_step//2

        if strategy == 'minimax' or strategy == 'expectimax':  
            p = Popen([program_path], stdout=PIPE, stdin=PIPE, stderr=PIPE)
            data_to_write = str(self.turn_step//2) + ' ' + (' '.join(' '.join(str(self.state.table[i][j]) for j in range(2)) for i in range(12)) + ' ' + str(self.state.player_point[0]) + ' ' + str(self.state.player_point[1]))
            data_to_write = data_to_write + ' ' + strategy + ' ' + str(depth) + ' '+ str(utility)
            stdout_data = p.communicate(input=data_to_write.encode())[0].decode()
            AI_move = list(map(int, stdout_data.split()))

        elif strategy == 'random':
            AI_move = getRandom(playerID, self.state.table)

    
        print(AI_move)
        self.state.update(self.turn_step//2, AI_move[0], AI_move[1], self.turn_step)
        # print(self.state)
        self.turn_step = (self.turn_step + 2) % 4
        