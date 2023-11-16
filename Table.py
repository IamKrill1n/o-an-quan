import pygame
import math
from variables import *
def get_nxt(i, direction):
    return (i + (1 if direction else -1)) % 12

class Table:
    def __init__(self):
        self.table =[[0, 1], [5, 0], [5, 0], [5, 0], [5, 0], [5, 0], [0, 1], [5, 0], [5, 0], [5, 0], [5, 0], [5, 0]]
        # self.table = [[7, 0], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0], [2, 0], [0 ,0], [7, 0], [0, 0], [4, 0], [1, 0]]
        self.selection = -1
        self.picking = -1
        self.empty_rows = [0, 0]
        self.player_point = [0, 0]

    def check_empty_rows(self, player):
        return sum(self.table[i][0] for i in range(1 + player * 6, 6 + player * 6)) == 0
                
    def render(self):    
        #draw semicircles
        pygame.draw.rect(screen, 'black', pygame.Rect(100, HEIGHT/2 - 100, WIDTH, 200))

        pygame.draw.arc(screen, 'light gray', pygame.Rect(100, HEIGHT/2-100, 200, 200), math.radians(90), math.radians(270), 10)
        pygame.draw.arc(screen, 'light gray', pygame.Rect(WIDTH - 300, HEIGHT/2-100, 200, 200), math.radians(270), math.radians(90), 10)
        dis = WIDTH - 400
        for i in range(6): # draw straight lines 
            pygame.draw.line(screen, 'light gray', (200 + dis/5*i, HEIGHT/2-100), (200 + dis/5*i, HEIGHT/2+100), width = 10)
        for i in range(3):
            pygame.draw.line(screen, 'light gray', (200, HEIGHT/2-100 + 100*i), (WIDTH-200, HEIGHT/2-100 + 100*i), width = 5)


        for i in range(1, 12):
            if i == 6: 
                continue
            w = 200 + dis/5 * (i-1 if i <= 5 else 11-i) + dis/10 - 3
            h = HEIGHT / 2 - 50 - 15 + 100 * (i <= 5)
            screen.blit(font.render(str(self.table[i][0]), True, 'white'), (w, h))
            
            if self.picking == i:
                if i <= 5:
                    pygame.draw.rect(screen, 'yellow', [200 + dis/5 * (i-1), HEIGHT / 2, dis/5, 100], 10)
                if i > 5:
                    pygame.draw.rect(screen, 'yellow', [200 + dis/5 * (11-i), HEIGHT / 2 - 100, dis/5, 100], 10)

            if self.selection == i: 
                if i <= 5:
                    pygame.draw.rect(screen, 'red', [200 + dis/5 * (i-1), HEIGHT / 2, dis/5, 100], 10)
                if i > 5:
                    pygame.draw.rect(screen, 'red', [200 + dis/5 * (11-i), HEIGHT / 2 - 100, dis/5, 100], 10)
            
        screen.blit(font.render((str(self.table[0])), True, 'white'), (130 , HEIGHT / 2 - 15))
        screen.blit(font.render((str(self.table[6])), True, 'white'), (WIDTH - 175 , HEIGHT / 2 - 15))
        pygame.display.update(pygame.Rect(100, HEIGHT/2 - 100, WIDTH, 200))

    def update(self, player, cell, direction): #ccw 1, cw 0
        self.selection, self.picking = cell, cell
        quan, self.table[self.selection][0] = self.table[self.selection][0], 0
        pygame.time.wait(delay)
        self.render()
        while(quan != 0):
            for i in range(quan):
                self.picking = get_nxt(self.picking, direction)
                self.table[self.picking][0] += 1

                pygame.time.wait(delay)
                self.render()

            self.picking = get_nxt(self.picking, direction)

            if self.table[self.picking][0] == 0:
                quan = 0
                while(self.table[self.picking][0] == 0): #eating nom nom
                    self.picking = get_nxt(self.picking, direction)
                    if(self.table[self.picking][0] != 0):
                        self.player_point[player] += self.table[self.picking][0] + self.table[self.picking][1] * 5
                        self.table[self.picking] = [0,0]
                        
                        pygame.time.wait(delay)
                        self.render()
                    else:
                        break
                    self.picking = get_nxt(self.picking, direction)
            elif self.picking == 0 or self.picking == 6: #stepped into 'quan' square, switch turn
                break
            elif self.table[self.picking][0] != 0:
                quan, self.table[self.picking][0] = self.table[self.picking][0], 0
                pygame.time.wait(delay)
                self.render()
        self.picking = -1
        self.selection = -1
        for i in range(2):
            self.empty_rows[i] = self.check_empty_rows(i)

    def borrow_point(self, playerID):  
        self.player_point[playerID] -= 5 
        for i in range(1 + playerID * 6, 6 + playerID * 6):
            self.table[i][0] = 1
            self.picking = i
            pygame.time.wait(delay)
            self.render()
        self.picking = -1
        self.empty_rows[playerID] = 0
    #def move(self, playerID, direction):

    

        