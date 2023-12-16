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

    def EndGameText(self):
        screen.fill('black')
        winner = 1 if self.state.player_point[0] > self.state.player_point[1] else 2
        text_1 = 'Player ' + str(winner) + ' has won the game with ' + str(self.state.player_point[winner-1]) + ' points!'
        if self.state.player_point[0] == self.state.player_point[1]:
            text_1 = 'The game has come to a draw'
        text_2 = 'Press enter or return to play again'
        screen.blit(big_font.render(text_1,  True, 'white'), ((WIDTH - big_font.size(text_1)[0]) / 2, 50))
        screen.blit(big_font.render(text_2,  True, 'white'), ((WIDTH - big_font.size(text_2)[0]) / 2, 125))
        pygame.display.flip()

    def render(self):
        if self.turn_step >= 4:
            return
        screen.fill('black')
        self.state.render()
        screen.blit(font.render('Player 1 point:' + str(self.state.player_point[0]), True, 'white'), (0, HEIGHT - 30))
        screen.blit(font.render('Player 2 point:' + str(self.state.player_point[1]), True, 'white'), (0, 0))
        status_text = ['Player 1: Select a square to play!', 'Use keyboard < or > to select a direction!',
                    'Player 2: Select a square to play!', 'Use keyboard < or > to Select a direction!', '']
        if self.turn_step < 2:
            screen.blit(big_font.render(status_text[self.turn_step], True, 'white'), ((WIDTH - big_font.size(status_text[self.turn_step])[0]) / 2, HEIGHT - 100))
        else:
            screen.blit(big_font.render(status_text[self.turn_step], True, 'white'), ((WIDTH - big_font.size(status_text[self.turn_step])[0]) / 2, 50))
        #creating an end state later ?
        pygame.display.flip()

    
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
            self.turn_step = (self.turn_step + 1) % 4
            self.state.update(playerID, cell, direction)

    def fix_empty_rows(self):
        if self.turn_step == 0 and self.state.empty_rows[0]:
            self.state.borrow_point(0)
        if self.turn_step == 2 and self.state.empty_rows[1]:
            self.state.borrow_point(1)
    
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
        self.state.update(self.turn_step//2, AI_move[0], AI_move[1])
        print(self.state)
        self.turn_step = (self.turn_step + 2) % 4
        