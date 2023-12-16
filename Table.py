import pygame
import os
import math
from variables import *
def get_nxt(i, direction):
    return (i + (1 if direction else -1)) % 12
assets = 'assets'
background = pygame.image.load(os.path.join(assets, 'background.png'))
def draw_text(text, x, y, font, color):
    text_surface = font.render(text, True, color)
    screen.blit(text_surface, (x, y))

class Table:
    def __init__(self):
        self.table =[[0, 1], [5, 0], [5, 0], [5, 0], [5, 0], [5, 0], [0, 1], [5, 0], [5, 0], [5, 0], [5, 0], [5, 0]]
        # self.table = [[0, 0], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0], [0 ,0], [7, 0], [0, 0], [4, 0], [1, 0]]
        self.selection = -1
        self.picking = -1
        self.empty_rows = [0, 0]
        self.player_point = [0, 0]

    def render(self, turn, method = None, val = 0):  
        screen.fill((255, 255, 255))
        screen.blit(background, (0, 0))
        if(turn == 4):
            winner = 1 if self.player_point[0] > self.player_point[1] else 2
            text_1 = 'Player ' + str(winner) + ' has won the game with ' + str(self.player_point[winner-1]) + ' points!'
            if self.player_point[0] == self.player_point[1]:
                text_1 = 'The game has come to a draw'
            text_2 = 'Press enter or return to play again'
            draw_text(text_1, (WIDTH - big_font.size(text_1)[0]) / 2, HEIGHT - 150, big_font, 'white')
            draw_text(text_2, (WIDTH - big_font.size(text_2)[0]) / 2, HEIGHT - 75, big_font, 'white')
        else:       
            dis = WIDTH - 400
            status_text = ['Player 1 is thinking...', 'Use keyboard < or > to select a direction!',
                        'Player 2 is thinking...', 'Use keyboard < or > to Select a direction!', '']
            size = big_font.size(status_text[turn])[0]

            if turn < 2:
                draw_text(status_text[turn], (WIDTH - size) / 2, HEIGHT - 120, big_font, 'white')
            else:
                draw_text(status_text[turn], (WIDTH - size) / 2, 50, big_font, 'white')

            for i in range(0, 12):
                w = 200 + dis/5 * (i-1 if i <= 5 else 11-i) + dis/10 - 3
                h = HEIGHT / 2 - 50 - 15 + 100 * (i <= 5)
                if i != 0 and i != 6:
                    draw_text(str(self.table[i][0]), w, h, font, 'white')
                if self.picking == i:
                    text = ''
                    if method == 'adding':
                        text = '+1'
                    elif method == 'eating' or method == 'taking':
                        text = '-' + str(val)
                    if i == 0 or i == 6:
                        draw_text(text, (130 - 90 if i == 0 else WIDTH - 175 + 105), HEIGHT / 2 - 15, font, ('red' if method == 'eating' else 'white'))
            
                    elif i <= 5:
                        draw_text(text, w - 6, h + 75, font, ('red' if method == 'eating' else 'white'))
                        pygame.draw.rect(screen, 'yellow', [200 + dis/5 * (i-1), HEIGHT / 2, dis/5, 100], 10)
                    elif i > 5:
                        draw_text(text, w - 6, h - 85, font, ('red' if method == 'eating' else 'white'))
                        pygame.draw.rect(screen, 'yellow', [200 + dis/5 * (11-i), HEIGHT / 2 - 100, dis/5, 100], 10)

                if self.selection == i: 
                    if i == 0 or i == 6:
                        print()
                    elif i <= 5:
                        pygame.draw.rect(screen, 'red', [200 + dis/5 * (i-1), HEIGHT / 2, dis/5, 100], 10)
                    elif i > 5:
                        pygame.draw.rect(screen, 'red', [200 + dis/5 * (11-i), HEIGHT / 2 - 100, dis/5, 100], 10)
                
            draw_text(str(self.table[0][1]), 115, HEIGHT / 2 - 45, font, 'white')
            draw_text(str(self.table[0][0]), 115, HEIGHT / 2 + 15, font, 'white')


            draw_text(str(self.table[6][1]), WIDTH - 125, HEIGHT / 2 - 45, font, 'white')
            draw_text(str(self.table[6][0]), WIDTH - 125, HEIGHT / 2 + 15, font, 'white')


            #drawing player point
            draw_text('Player 1 point:' + str(self.player_point[0]), 0, HEIGHT - 40, font, 'white')
            draw_text('Player 2 point:' + str(self.player_point[1]), 0, 0, font, 'white')

            #drawing if eating
            if method == 'eating':
                text = '+' + str(val)
                    
                if turn < 2:
                    draw_text(text, 250, HEIGHT - 40, font, 'white')
                else:
                    draw_text(text, 250, 0, font, 'white')
        pygame.display.flip()

    def redraw(self, turn, method = None, val = 0):
        self.render(turn, method, val)
    def check_empty_rows(self, player):
        return sum(self.table[i][0] for i in range(1 + player * 6, 6 + player * 6)) == 0
            
    def blink(self, turn, method = None, val = 0):
        self.redraw(turn, method, val)
        pygame.time.wait(delay)
        

    def update(self, player, cell, direction, turn): #ccw 1, cw 0
        self.selection, self.picking = cell, cell
        quan, self.table[self.selection][0] = self.table[self.selection][0], 0
        self.blink(turn)
        while(quan != 0):
            for i in range(quan):
                self.picking = get_nxt(self.picking, direction)
                self.table[self.picking][0] += 1

                self.blink(turn, 'adding')
            self.picking = get_nxt(self.picking, direction)

            if self.table[self.picking][0] == 0:
                quan = 0
                while(self.table[self.picking][0] == 0): #eating nom nom
                    self.picking = get_nxt(self.picking, direction)
                    if(self.table[self.picking][0] != 0):
                        val = self.table[self.picking][0] + self.table[self.picking][1] * QUANVALUE
                        self.player_point[player] += val
                        self.table[self.picking] = [0,0]
                        
                        self.blink(turn, 'eating', val)
                    else:
                        break
                    self.picking = get_nxt(self.picking, direction)
            elif self.picking == 0 or self.picking == 6: #stepped into 'quan' square, switch turn
                break
            elif self.table[self.picking][0] != 0:
                val = self.table[self.picking][0]
                quan, self.table[self.picking][0] = self.table[self.picking][0], 0
                self.blink(turn, 'taking', val)
        self.picking = -1
        self.selection = -1
        for i in range(2):
            self.empty_rows[i] = self.check_empty_rows(i)

    def borrow_point(self, playerID, turn):  
        self.player_point[playerID] -= 5 
        for i in range(1 + playerID * 6, 6 + playerID * 6):
            self.table[i][0] = 1
            self.picking = i
            self.blink(turn)

        self.picking = -1
        self.empty_rows[playerID] = 0

    def __str__(self): 
        return '''
            11 10  9  8  7  6 
        +--+--------------+--+
        |{:2}|{:2}|{:2}|{:2}|{:2}|{:2}|{:2}|
        |{:2}|--------------|{:2}|
        |  |{:2}|{:2}|{:2}|{:2}|{:2}|  |
        +--+--------------+--+
          0  1  2  3  4  5

        USER_0: {} USER_1: {}
        '''.format(
            " *" if self.table[0][1] == 1 else " ",
                self.table[11][0] if self.table[11][0] else '',
                self.table[10][0] if self.table[10][0] else '',
                self.table[9][0] if self.table[9][0] else '',
                self.table[8][0] if self.table[8][0] else '',
                self.table[7][0] if self.table[7][0] else '',
                " *" if self.table[6][1] == 1 else " ",  
                self.table[0][0] if self.table[0][0] else '',
                self.table[6][0] if self.table[6][0] else '',
                self.table[1][0] if self.table[1][0] else '',
                self.table[2][0] if self.table[2][0] else '',
                self.table[3][0] if self.table[3][0] else '',
                self.table[4][0] if self.table[4][0] else '',
                self.table[5][0] if self.table[5][0] else '',
                self.player_point[0], self.player_point[1]
        )

    

        