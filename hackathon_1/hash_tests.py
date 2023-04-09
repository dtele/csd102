import random
import time


def hash1(ticket):
    ticket_str = str(ticket)
    sum = 0
    for i in range(0, len(ticket_str), 2):
        sum += int(ticket_str[i:i + 2])

    return sum


def hash2(ticket):
    sum = 0
    tix = str(ticket)
    for i in range(2, 6):
        sum += int(tix[i]) * (10 ** (6 - i - 1))
    return sum // 3


def benchmark(tickets, hash_func):
    hash_map = {}
    collisions = 0
    time_taken = time.time()

    for ticket in tickets:
        hashed_ticket = hash_func(ticket)
        if hashed_ticket in hash_map.keys():
            hash_map[hashed_ticket] += 1
            collisions += 1
        else:
            hash_map[hashed_ticket] = 1

    time_taken = time.time() - time_taken

    return time_taken, collisions, min(hash_map.keys()), max(hash_map.keys())


tickets = random.sample(range(100000, 200000), k=1000)
hash1_benchmark = benchmark(tickets, hash1)
hash2_benchmark = benchmark(tickets, hash2)

print(f'hash function 1\ntime taken to hash all tickets: {hash1_benchmark[0]}s\nnumber of collisions: {hash1_benchmark[1]}\nrange: [{hash1_benchmark[2]} : {hash1_benchmark[3]}]\n')
print(f'hash function 2\ntime taken to hash all tickets: {hash2_benchmark[0]}s\nnumber of collisions: {hash2_benchmark[1]}\nrange: [{hash2_benchmark[2]} : {hash2_benchmark[3]}]\n')
