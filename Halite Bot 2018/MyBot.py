import hlt
import logging
from collections import OrderedDict

# GAME START
game = hlt.Game("lancerV2")
logging.info("Initializing lancerV2")
currentRound = 1

while True:

    game_map = game.update_map()
    myID = game_map.get_me().id     #obtaining the player's id
    #logging.info(currentRound)

    team_ships = game_map.get_me().all_ships()  #creating an array storing all ships the owner commands in the game

    command_queue = []

    for ship in game_map.get_me().all_ships():  #For every ship the player owns
        target = []
        planetOwner = -1

        if ship.docking_status != ship.DockingStatus.UNDOCKED:  #As long as the ship is undocked, continue
            continue

        entities_by_distance = game_map.nearby_entities_by_distance(ship)   #Obtaining all objects on the map with their respective distance

        #sorted
        entities_by_distance = OrderedDict(sorted(entities_by_distance.items(), key=lambda t: t[0]))    #Sorting the values found above by distance

        closestEmptyPlanets = [entities_by_distance[distance][0] for distance in entities_by_distance if isinstance(entities_by_distance[distance][0], hlt.entity.Planet) and not entities_by_distance[distance][0].is_full()]  #Creating an array of the entities found above that are unfilled planets

        closestEnemyShips = [entities_by_distance[distance][0] for distance in entities_by_distance if  #Finding all the entities that are enemy ships
                             isinstance(entities_by_distance[distance][0], hlt.entity.Ship) and
                             entities_by_distance[distance][0] not in team_ships]

        if closestEmptyPlanets: #If there are empty planets on the map
            #logging.info("stage1")
            if closestEmptyPlanets[0].owner is not None:    #If the planet has an owner
                #logging.info("stage2")
                planetOwner = closestEmptyPlanets[0].owner.id
            else:   #otherwise if the planet does not have an owner
                #logging.info("stage3")
                planetOwner = -1


        #logging.info(closestEmptyPlanets)
        #logging.info(len(closestEmptyPlanets))
        #logging.info(planetOwner)

        if len(closestEmptyPlanets) > 0 and closestEmptyPlanets[0].owner is None or planetOwner == myID: #If there are planets that are not full that are either owned by the player or empty
            logging.info("expansion")
            target = closestEmptyPlanets[0] #The target is set to the closetest not full planet

            distanceFromPlanet = ship.calculate_distance_between(closestEmptyPlanets[0])    #Calculating the distance from the target planet
            distanceFromEnemy = ship.calculate_distance_between(closestEnemyShips[0])   #Calculating the distance from the closest enemy

            if distanceFromPlanet <= distanceFromEnemy: #If the distance distance to the planet is closer
                if ship.can_dock(target):   #Dock if the ship can dock
                    command_queue.append(ship.dock(target))
                else:   #or navigate closer to the planet so it may dock
                    navigate_command = ship.navigate(ship.closest_point_to(target), game_map, speed=int(hlt.constants.MAX_SPEED), ignore_ships=False)

                    if navigate_command:
                        command_queue.append(navigate_command)
            else:   #If the distance to the enemy is closer
                target = closestEnemyShips[0]   #The target for the current player ship switches to the enemy
                navigate_command = ship.navigate(ship.closest_point_to(target), game_map, speed=int(hlt.constants.MAX_SPEED), ignore_ships=False)   #the ship will begin to navigate towards the enemy

                if navigate_command:
                    command_queue.append(navigate_command)
        elif len(closestEnemyShips) > 0:    #If there are no planets that are not full, and there are enemy ships to target
            logging.info("attack V2")
            target_ship = closestEnemyShips[0]  #The target for the current player ship is switched to the closets enemy
            navigate_command = ship.navigate(ship.closest_point_to(target_ship), game_map,
                                             speed=int(hlt.constants.MAX_SPEED), ignore_ships=False)

            if navigate_command:
                command_queue.append(navigate_command)
        else:   #Default case if neither of the above are true is to attack the closest enemy ship
            logging.info("attack V3")
            target_ship = closestEnemyShips[0]
            navigate_command = ship.navigate(ship.closest_point_to(target_ship), game_map,
                                             speed=int(hlt.constants.MAX_SPEED), ignore_ships=False)

            if navigate_command:
                command_queue.append(navigate_command)

    currentRound = currentRound + 1 #iterating the current round
    game.send_command_queue(command_queue)  #Sending commands