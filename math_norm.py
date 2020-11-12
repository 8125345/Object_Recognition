import numpy as np
import math


def cal_move(agent_pos, agent_v, action, ships, s):
    action -= 10
    pre_pos = agent_pos
   
    if s == 1:
        agent_pos = [250, agent_pos[1] + 15 * action]
    elif s==2:
        agent_pos = [450, agent_pos[1] + 15 * action]
    else:
        agent_pos = [540, 440]
    
    success, agent_v = DD(pre_pos, agent_pos, agent_v, ships)
    return success, agent_pos, agent_v


def DD(p0, p1, v0, pGShip):

    v = np.zeros((2,10001))
    v[:,0] = v0
    p = np.zeros((2,10001))
    p[:,0] = p0
    rGShip = 70
    Pcnt = 0

    i = 0

    while (p[0, i] - p1[0] < 0) and (i < 10000) and v[0, i] > 0:
        if (np.linalg.norm(p[:, i] - pGShip[:, 0]) < rGShip) or (np.linalg.norm(p[:, i] - pGShip[:, 1]) < rGShip):
            Pcnt = Pcnt + 1
        print(i)
        i = i + 1

        p[:, i] = p[:, i - 1] + v[:, i - 1]
        print('p[:, i]: ', p[:, i])
        alpha = math.atan((p1[1] - p[1, i - 1]) / (p1[0] - p[0, i - 1]))
        print('atan: ', (p1[1] - p[1, i - 1]) / (p1[0] - p[0, i - 1]))
        print('alpha: ',alpha)
        vt = np.zeros((2, 1))
        vt[0, 0] = math.cos(alpha) * v[0, i - 1] + math.sin(alpha) * v[1, i - 1]
        vt[1, 0] = -math.sin(alpha) * v[0, i - 1] + math.cos(alpha) * v[1, i - 1]

        D = np.linalg.norm(p[:, i - 1] - p1)
        print('D: ', D)

        a2 = -vt[1, 0] / D * 3 * np.linalg.norm(v[:, i - 1])

        a1 = 3 * np.exp(-6) * np.linalg.norm(v[:, i - 1]) * np.linalg.norm(v[:, i - 1]) - 0.5 * np.abs(a2)

        a0 = np.zeros((2, 1))
        a0[0, 0] = -a1
        a0[1, 0] = a2

        a = np.zeros((2, 1))
        a[0, 0] = math.cos(alpha) * a0[0, 0] - math.sin(alpha) * a0[1, 0]
        a[1, 0] = math.sin(alpha) * a0[0, 0] + math.cos(alpha) * a0[1, 0]

        v[0, i] = v[0, i - 1] + a[0, 0]
        v[1, i] = v[1, i - 1] + a[1, 0]




    print('Pcnt: ',Pcnt);
    print("i: ",i)
    p_block = (Pcnt > 0)
    # print(Pcnt)
    v_end1 = np.linalg.norm(v[:, i])
    v_end2 = np.linalg.norm(v[:, i - 1])
    print(v[:, i])
    print(v[:, i - 1])
    print('v_end1: ', v_end1);
    print('v_end2: ', v_end2);
    # v_end = max([v_end1, v_end2])
    num = np.argmax(np.array([v_end1, v_end2]))
    v_end = v[:, i - num]
    d_end1 = np.linalg.norm(p[:, i] - p1)
    d_end2 = np.linalg.norm(p[:, i - 1] - p1)
    d_end = min([d_end1, d_end2])
    print('p_block: ', p_block);
    print('num: ', num);
    print('d_end: ', d_end);



    if p_block is False and d_end < 10:
        success = True
    else:
        success = False
    return success, v_end



# v = [[1,2,3,4],[5,6,7,8]]
# v1 = np.array(v)
# print("v1 is ", v1)
# v0 = v1[:, 0]
# print("v0 is ", v0)
#
# s = [[9,10,11,12],[13,14,15,16]]
# s1 = np.array(s)
# print("s1 is ",s1)
# s0 = s1[:,0]
# print("s0 is ", s0)
#
# result = v1[:, 0] - s1[:, 0]
# print("result is ", result)
#
# flag = np.linalg.norm(result)
#
# print("flag is ", flag)

# v_end1 = 11
# v_end2 = 10
#  # v_end = max([v_end1, v_end2])
# num = np.argmax(np.array([v_end1, v_end2]))
# print(num)
##p[:, i] = p[:, i - 1] + v[:, i - 1]
v = [[1,2,3,4],[5,6,7,8]]
s = [[9,10,11,12],[13,14,15,16]]
s1 = np.array(s)
v1 = np.array(v)

v1[:, 3] = v1[:, 3 - 1] + s1[:, 3 - 1]
print("v1 is ", v1)





# agent_pos = [50,200]
# agent_pos_1 = np.array(agent_pos)
# agent_v = [10,0]
# agent_v_1 = np.array(agent_v)
# action= 13
# ships = [[310,320],[400,450]]
# ships_1 = np.array(ships)
# s = 1
#
#
#
#
# print(cal_move(agent_pos_1,agent_v_1,action,ships_1,s))
