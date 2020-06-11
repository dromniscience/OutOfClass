D, R, U, L = 0, 1, 2, 3

def interpret(s):
    if s == 0:
        return 'D'
    elif s == 1:
        return 'R'
    elif s == 2:
        return 'U'
    elif s == 3:
        return 'L'
    else:
        raise ValueError("Invalid Operation on the Cube!")

class Cube:
    def __init__(self, state: tuple, corner: tuple):
        self.state = state
        self.corner = corner

    def __Turn(self, dir):
        if dir not in {D, R, U, L}:
            raise ValueError("Invalid Operation on the Cube!")
        self.state = list(self.state)
        self.corner = list(self.corner)
        if dir == D:
            self.state[0], self.state[2] = 1-self.state[2], 1-self.state[0]
            self.state[1] = 1-self.state[1]
            self.corner[0], self.corner[1] = self.corner[1], self.corner[0]
        elif dir == R:
            self.state[2], self.state[4] = 1-self.state[4], 1-self.state[2]
            self.state[3] = 1-self.state[3]
            self.corner[1], self.corner[2] = self.corner[2], self.corner[1]
        elif dir == U:
            self.state[4], self.state[6] = 1-self.state[6], 1-self.state[4]
            self.state[5] = 1-self.state[5]
            self.corner[2], self.corner[3] = self.corner[3], self.corner[2]
        else:
            self.state[6], self.state[0] = 1-self.state[0], 1-self.state[6]
            self.state[7] = 1-self.state[7]
            self.corner[3], self.corner[0] = self.corner[0], self.corner[3]
        self.state = tuple(self.state)
        self.corner = tuple(self.corner)

    def Turn(self, dir):
        temp = Cube(self.state, self.corner)
        temp.__Turn(dir)
        return temp

    def __eq__(self, other):
        return self.state == other.state and self.corner == other.corner

    def __hash__(self):
        return hash((self.state, self.corner))


init = Cube((0,)*8, (0,1,2,3))
status = {}

import queue
myque = queue.Queue()

myque.put((0, init))
status[init] = []

while not myque.empty():
    top = myque.get()
    for i in range(4):
        cube = top[1].Turn(i)
        if cube not in status:
            status[cube] = [interpret(i)] + status[top[1]]
            myque.put((top[0]+1, cube))

while True:
    state = tuple(map(int, input().split()))
    corner = tuple(map(int, input().split()))
    print(status[Cube(state, corner)])
