import pygame 
pygame.init()
WIDTH = 1000
HEIGHT = 700
font_size = 30
font_size_big = 50
screen = pygame.display.set_mode([WIDTH, HEIGHT])
font = pygame.font.Font('DTM-Sans.ttf', font_size)
big_font = pygame.font.Font('DTM-Sans.ttf', font_size_big)
QUANVALUE = 5
timer = pygame.time.Clock()
fps = 120
screen_color = (0, 0, 0)
#gameState = 'start'
# game variables and images
delay = 400
#0 - player 1 turn no selection: 1- player 1 turn square selected: 2- player 2 turn no selection: 3- player 2 turn selected square
piece = pygame.transform.scale(pygame.image.load('assets/rock.png'), (80,80))