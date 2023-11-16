from Table import *
from subprocess import Popen, PIPE


def get_coord(x, y):
    sol = (y - (HEIGHT/2 - 100)) // 100 * 5
    if sol == 0: #player 2's side
        return 11 - (x-200) // ((WIDTH - 400) / 5)
    else: #player 1's side
        return  (x-200) // ((WIDTH - 400) / 5) + 1

class Game:
    def __init__(self):
        self.table = Table()
        self.turn_step = 0
        self.AI_move = None

    def EndGameText(self):
        screen.fill('black')
        winner = 1 if self.table.player_point[0] > self.table.player_point[1] else 2
        text_1 = 'Player ' + str(winner) + ' has won the game with ' + str(self.table.player_point[winner-1]) + ' points!'
        text_2 = 'Press enter or return to play again'
        screen.blit(big_font.render(text_1,  True, 'white'), ((WIDTH - big_font.size(text_1)[0]) / 2, 50))
        screen.blit(big_font.render(text_2,  True, 'white'), ((WIDTH - big_font.size(text_2)[0]) / 2, 125))
        pygame.display.flip()

    def render(self):
        screen.fill('black')
        self.table.render()
        screen.blit(font.render('Player 1 point:' + str(self.table.player_point[0]), True, 'white'), (0, HEIGHT - 30))
        screen.blit(font.render('Player 2 point:' + str(self.table.player_point[1]), True, 'white'), (0, 0))
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
        if 0 < cur < 12 and self.table.table[cur] != [0,0]:
            if (self.turn_step <= 1 and 0 < cur < 6) or (self.turn_step > 1 and 6 < cur < 12):
                self.table.selection, self.table.picking = cur, cur
                self.turn_step = 1 if self.turn_step <= 1 else 3

    def play(self, cell, event): #find direction and play
        if self.turn_step == 1 or self.turn_step == 3:
            playerID = self.turn_step//2
            #1 is ccw, 0 is cw
            #for player 1, going left is cw, right is ccw
            #for player 2, going left is ccw, right is cw   
            direction = (playerID + (event == pygame.K_RIGHT)) % 2
            self.turn_step = (self.turn_step + 1) % 4
            self.table.update(playerID, cell, direction)

    def fix_empty_rows(self):
        if self.turn_step == 0 and self.table.empty_rows[0]:
            self.table.borrow_point(0)
        if self.turn_step == 2 and self.table.empty_rows[1]:
            self.table.borrow_point(1)
    
    def game_over(self):
        if self.table.table[0] == [0,0] and self.table.table[6] == [0,0]:
            self.table.player_point[0] += sum(self.table.table[i][0] for i in range(1, 6))
            self.table.player_point[1] += sum(self.table.table[i][0] for i in range(7, 12))
            for i in range(12):
                self.table.table[i] = [0, 0]
            return True
        return False

    def make_AI_move(self): #print cell and direction
        
        program_path = "bot.exe"
        p = Popen([program_path], stdout=PIPE, stdin=PIPE, stderr=PIPE)
        data_to_write = str(self.turn_step//2) + ' ' + (' '.join(' '.join(str(self.table.table[i][j]) for j in range(2)) for i in range(12)) + ' ' + str(self.table.player_point[0]) + ' ' + str(self.table.player_point[1]))
        stdout_data = p.communicate(input=data_to_write.encode())[0].decode()
        AI_move = list(map(int, stdout_data.split()))
        print(AI_move)
        self.table.update(self.turn_step//2, AI_move[0], AI_move[1])
        self.turn_step = (self.turn_step + 2) % 4
