# PROJECT
MQA

# THEME
## Genre
- Casual, Time Management
## Setting
- Modern household, different rooms and mess types

## Mood
- Light, fun, mildly challenging

# Idea
- Player is tasked with cleaning up various messes around a house.
Each level present a new house (or room). The player must interact with and clean items quickly to achieve the highest score possible.

## Goal
1. Primary: Achieve at least 70% cleanliness within given time.
2. Secondary:
- Achieve high scores
- Collect hidden or challenging-to-reach items to earn bonus points/easter eggs

# Core Mechanics

## Movement
- Move around the house with arrow keys or similar setups

## Cleaning
- Cleaning messes involves approaching and pressing an interaction key, which may trigger animations, minigame, popup,... 

## Time Management
- Each cleaning task takes an amount of time, base on tool stat, mess type
- Power-ups and time-extenders can be unlocked to help player manage time better

## Obstacles
- A cat, which can run and trip player off

## Scoring
- Each mess type and minigame have base score. Player get score by combining mess type score and minigame net score.
    - For example: Each dust pile score 10, button_prompt score max 20. each button misses decrease max by 5. After finishing the minigame,
    player score are 10 + (20 - 5 * misses)
- A map calculate the max score player can get and make 1 star, 2 star, 3 star respectively with 50%, 70% and 90% max score.
- Player can surpass max score, maybe by streak/combo, hidden item,...

## Details

### Mini-game
- **Button-prompt**
    - Player press correct button shown on screen 
- **Rhythm**
    - Player press one button in time
- **Balance**
    - Player press left and right to balance a meter
- **Rapid Tap**
    - Player rapidly tap one button

### Messes types

**TODO**: Should minigame be randomized?
1. Dust
- Requirement: Broom
- Trigger: Button-Promt

2. Stains
- Requirement: Cloth
- Trigger: Rapid Tap

3. Dishes
- Requirement: None
- Trigger: Rhythm

4. Spills
- Requirement: Mop
- Trigger: Balance




