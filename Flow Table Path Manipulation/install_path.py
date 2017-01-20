#!/usr/bin/python

import sys
import re # For regex

import ryu_ofctl
from ryu_ofctl import *

def main(macHostA, macHostB):
    print "Installing flows for %s <==> %s" % (macHostA, macHostB)

    ##### FEEL FREE TO MODIFY ANYTHING HERE #####
    try:
        pathA2B = dijkstras(macHostA, macHostB)
        installPathFlows(macHostA, macHostB, pathA2B)
    except:
        raise


    return 0

# Installs end-to-end bi-directional flows in all switches
def installPathFlows(macHostA, macHostB, pathA2B):

    #TODO: FIX THIS
    def installPathFlowHelper(destination, path_dict, back=False):
        flow = FlowEntry()

        if back:
            act = OutputAction(int(path_dict["in_port"]))
            flow.in_port = int(path_dict["out_port"])
        else:
            act = OutputAction(int(path_dict["out_port"]))
            flow.in_port = int(path_dict["in_port"])

        flow.dl_dst = str(destination)
        flow.addAction(act)
        
        dpid = int(path_dict["dpid"])
        insertFlow(dpid, flow)

    for path in pathA2B:
        print "path ", path
        print "macHostB ", macHostB
        installPathFlowHelper(macHostB, path)

    #Has to be bidirectional
    for backpath in pathA2B[::-1]:
        print "backpath ", backpath
        print "macHostA ", macHostA
        installPathFlowHelper(macHostA, backpath, back=True)

    return

# Returns List of neighbouring DPIDs
def findNeighbours(dpid):
    if type(dpid) not in (int, long) or dpid < 0:
        raise TypeError("DPID should be a positive integer value")
    if dpid not in listSwitches()['dpids']:
        print "DPIDs " + listSwitches()
        raise TypeError("input DPID not in existing DPIDs")

    neighbours = []

    for link in listSwitchLinks(dpid)['links']:
        endpoint1_dpid = link['endpoint1']['dpid']
        endpoint2_dpid = link['endpoint2']['dpid']

        if endpoint1_dpid != dpid and endpoint1_dpid not in neighbours:
            neighbours += [endpoint1_dpid]
        if endpoint2_dpid != dpid and endpoint2_dpid not in neighbours:
            neighbours += [endpoint2_dpid]

    return neighbours


# returns port number of previous node to this node
def get_inport(prev_node, node):
    links = listSwitchLinks(int(node))['links']
    for link in links:
        if link['endpoint2']['dpid'] == int(prev_node):
            return int(link['endpoint1']['port'])

    raise TypeError("These nodes do not connect - %s and %s" % (prev_node, node))


# returns port number of this node to next node
def get_outport(node, next_node):
    links = listSwitchLinks(int(node))['links']
    for link in links:
        if link['endpoint1']['dpid'] == int(next_node):
            return int(link['endpoint2']['port'])

    raise TypeError("These nodes do not connect - %s and %s" % (node, next_node))


# Calculates least distance path between A and B
# Returns detailed path (switch ID, input port, output port)
#   - Suggested data format is a List of Dictionaries
#       e.g.    [   {'dpid': 3, 'in_port': 1, 'out_port': 3},
#                   {'dpid': 2, 'in_port': 1, 'out_port': 2},
#                   {'dpid': 4, 'in_port': 3, 'out_port': 1},
#               ]
# Raises exception if either ingress or egress ports for the MACs can't be found
def dijkstras(macHostA, macHostB):

    # Optional helper function if you use suggested return format
    def nodeDict(dpid, in_port, out_port):
        assert type(dpid) in (int, long)
        assert type(in_port) is int
        assert type(out_port) is int
        return {'dpid': dpid, 'in_port': in_port, 'out_port': out_port}

    def get_lowest_node(distanceFromA_dict, queue_list):
        lowest = float('inf')
        for queue_object in queue_list:
            if distanceFromA_dict[str(queue_object)] < lowest:
                lowest = distanceFromA_dict[str(queue_object)]
                low_node = str(queue_object)

        #print "lowest node = ", low_node
        return int(low_node)


    def make_path_from_node_list(node_list):
        start_port = getMacIngressPort(macHostA)['port']
        end_port = getMacIngressPort(macHostB)['port']

        path = []
        if len(node_list) != 1:
            for nindex, node in enumerate(node_list):

                if nindex == 0:
                    outport = get_outport(node, node_list[nindex + 1])
                    path += [nodeDict(int(node), int(start_port), int(outport))]

                elif nindex == (len(node_list) - 1):
                    inport = get_inport(node_list[nindex - 1], node)
                    path += [nodeDict(int(node), int(inport), int(end_port))]

                else:
                    inport = get_inport(node_list[nindex - 1], node)
                    outport = get_outport(node, node_list[nindex + 1])
                    path += [nodeDict(int(node), int(inport), int(outport))]
        else:
            path += [nodeDict(int(node_list[0]), int(start_port), int(end_port))]

        return path

    #TODO: TAKE CARE OF CORNER CASE WITH JUST ONE NODE

    #print "Mac Host A ", str(getMacIngressPort(macHostA)['dpid'])
    #print "Mac Host B ", str(getMacIngressPort(macHostB)['dpid'])

    if getMacIngressPort(macHostA) == None:
        raise TypeError("macHostA does not exist")
    if getMacIngressPort(macHostB) == None:
        raise TypeError("macHostB does not exist")
    if getMacIngressPort(macHostA) == getMacIngressPort(macHostB):
        return make_path_from_node_list([int(getMacIngressPort(macHostA))])

    # Optional variables and data structures
    distance_between_nodes = 1
    INFINITY = float('inf')
    distanceFromA = {} # Key = node, value = distance
    leastDistNeighbour = {} # Key = node, value = neighbour node with least distance from A
    predecessor = {str(getMacIngressPort(macHostA)['dpid']):None} # Holds path information
    visited = [str(getMacIngressPort(macHostA)['dpid'])]
    pathAtoB = []

    queue = listSwitches()['dpids']

    for vertex in queue:
        distanceFromA[str(vertex)] = INFINITY
    distanceFromA[str(getMacIngressPort(macHostA)['dpid'])] = 0
#    queue.remove(int(getMacIngressPort(macHostA)['dpid']))
    #print "queue ", queue

    while len(queue) != 0 and int(getMacIngressPort(macHostB)['dpid']) in queue:

        lowest_node = get_lowest_node(distanceFromA, queue)
        #print "lowest node ", lowest_node
        #print "queue ", queue
        queue.remove(lowest_node)

        for neighbour in findNeighbours(int(lowest_node)):
            temp_distance = distanceFromA[str(lowest_node)] + distance_between_nodes
            if temp_distance < distanceFromA[str(neighbour)]:
                distanceFromA[str(neighbour)] = temp_distance
                predecessor[str(neighbour)] = lowest_node

    current_node = str(getMacIngressPort(macHostB)['dpid'])
    pathAtoB.insert(0, current_node)
    
    #print "predecessor ", predecessor

    while current_node != str(getMacIngressPort(macHostA)['dpid']):
        current_node = str(predecessor[str(current_node)])
        pathAtoB.insert(0, current_node)

    print "path A to B ", pathAtoB

    return make_path_from_node_list(pathAtoB)



# Validates the MAC address format and returns a lowercase version of it
def validateMAC(mac):
    invalidMAC = re.findall('[^0-9a-f:]', mac.lower()) or len(mac) != 17
    if invalidMAC:
        raise ValueError("MAC address %s has an invalid format" % mac)

    return mac.lower()

if __name__ == '__main__':
    if len(sys.argv) != 3:
        print "This script installs bi-directional flows between two hosts"
        print "Expected usage: install_path.py <hostA's MAC> <hostB's MAC>"
    else:
        macHostA = validateMAC(sys.argv[1])
        macHostB = validateMAC(sys.argv[2])

        sys.exit( main(macHostA, macHostB) )
