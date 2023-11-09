import pygame 
pygame.init()
WIDTH = 1000
HEIGHT = 700
screen = pygame.display.set_mode([WIDTH, HEIGHT])
font = pygame.font.Font('DTM-Sans.ttf', 30)
big_font = pygame.font.Font('DTM-Sans.ttf', 50)
timer = pygame.time.Clock()
fps = 60
screen_color = (0, 0, 0)
gameState = 'start'
# game variables and images

player_point = [0, 0]
#0 - player 1 turn no selection: 1- player 1 turn square selected: 2- player 2 turn no selection: 3- player 2 turn selected square
turn_step = 0
piece = pygame.transform.scale(pygame.image.load('assets/rock.png'), (80,80))