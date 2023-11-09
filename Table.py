import pygame
import math
from variables import *

class Table:
    def __init__(self):
        #self.table =[[0, 1], [5, 0], [5, 0], [5, 0], [5, 0], [5, 0], [0, 1], [5, 0], [5, 0], [5, 0], [5, 0], [5, 0]]
        self.table = [[7, 0], [0, 0], [0, 0], [0, 0], [0, 0], [0, 0], [2, 0], [0 ,0], [7, 0], [0, 0], [4, 0], [1, 0]]
        self.selection = 100
        self.picking = 100
    def empty_rows(self, player):
        return sum(self.table[i][0] for i in range(1 + (1-player) * 6, 6 + (1-player) * 6)) == 0
    def render(self):    
        #draw semicircles
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
            w = 200 + dis/5 * ((i-1 if i <= 5 else 11-i) % 5) + dis/10 - 3
            h = HEIGHT / 2 - 50 - 15 + 100 * (i > 5)
            screen.blit(font.render(str(self.table[i][0]), True, 'white'), (w, h))
            
#            if turn_step < 2 and i == 1 or turn_step >= 2 and i == 0:
            if self.picking == i:
                if i <= 5:
                    pygame.draw.rect(screen, 'yellow', [200 + dis/5 * ((i-1)%5), HEIGHT / 2 + 100*((i > 5) - 1), dis/5, 100], 10)
                if i > 5:
                    pygame.draw.rect(screen, 'yellow', [200 + dis/5 * ((11-i)%5), HEIGHT / 2 + 100*((i > 5) - 1), dis/5, 100], 10)

            if self.selection == i: 
                if i <= 5:
                    pygame.draw.rect(screen, 'red', [200 + dis/5 * ((i-1)%5), HEIGHT / 2 + 100*((i > 5) - 1), dis/5, 100], 10)
                if i > 5:
                    pygame.draw.rect(screen, 'red', [200 + dis/5 * ((11-i)%5), HEIGHT / 2 + 100*((i > 5) - 1), dis/5, 100], 10)
            
        screen.blit(font.render((str(self.table[0])), True, 'white'), (130 , HEIGHT / 2 - 15))
        screen.blit(font.render((str(self.table[6])), True, 'white'), (WIDTH - 175 , HEIGHT / 2 - 15))