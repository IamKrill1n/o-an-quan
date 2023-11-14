import pygame
import sys 
from variables import *
from Game import Game
import math
pygame.init()
gameState = 'start'
###MORE PIECES ? TAKE CARE LATER.
playState = Game()
pygame.display.set_caption('oanquantet')
#main game loop
#ccw 0, cw 1
   
run = True
while run:
    timer.tick(fps)
    #event handling - capturing everything going on your keyboard
    
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            run = False
            sys.exit()
        if event.type == pygame.KEYDOWN and (event.key == pygame.K_RETURN or event.key == pygame.K_KP_ENTER):
            if gameState == 'end':
                playState = Game()
                player_point = [0, 0]
                gameState = 'start'
        if gameState == 'end': 
            continue
        playState.fix_empty_rows()
        if event.type == pygame.MOUSEBUTTONDOWN and event.button == 1: #check left mouse click
            playState.check_valid_move(event)
        
        if event.type == pygame.KEYDOWN and playState.selection != -1 and (event.key == pygame.K_LEFT or event.key == pygame.K_RIGHT):  
            playState.play(event.key)
        playState.render()
    
    if playState.game_over():
        gameState = 'end'
        playState.EndGameText()
        playState.turn_step = 4
        
pygame.quit()