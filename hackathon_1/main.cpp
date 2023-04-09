#include <iostream>
#include <random>

#define MAXTICKET 200000
#define MINTICKET 100000
#define TICKETCOUNT 1000
#define TABLESIZE 3334

using namespace std;


struct Ticket {
    int id;
    bool entered;

    explicit Ticket(int ticket_id = -1) {
        id = ticket_id;
        entered = false;
    }
};


class EnteredTickets {
public:
    Ticket stack[TICKETCOUNT];
    int ix;

    EnteredTickets() {
        ix = 0;
    }

    void push(Ticket ticket) {
        if (ix == TICKETCOUNT) {
            return;
        }
        stack[ix] = ticket;
        ix++;
    }

    int count() const {
        return ix;
    }
};


Ticket valid_tickets[TICKETCOUNT];
Ticket hash_table[TABLESIZE];
EnteredTickets entered_tickets;


unsigned int hash_id(int id) {
    // hash function: (last 4 digits of 6 digit number) // 3
    // domain: [100000:200000]
    // range: [0:3333]
    // collision chance: ~12 per 100

    char digits[6];
    sprintf(digits, "%d", id);

    int prehash = 0;

    for (int i = 2; i < 6; i++) {
        prehash += (int)((digits[i] - '0') * (pow(10, 6 - i - 1)));
    }

    int hashed = prehash / 3;

    return hashed;
}


void number_of_people() {
    if (entered_tickets.count() == 0) cout << "Nobody has entered the festival yet." << endl;
    else if (entered_tickets.count() == 1) cout << "1 person has entered the festival." << endl;
    else if (entered_tickets.count() > 1) cout << entered_tickets.count() << " people have entered the festival." << endl;
}


void insert_table(unsigned int index, Ticket ticket) {
    if (hash_table[index].id == -1) {
        hash_table[index].id = ticket.id;
    } else {
        int fails = 1;
        while (hash_table[index].id != -1) {
            index += (fails * fails) % TABLESIZE;
            fails++;
        }
        hash_table[index].id = ticket.id;
    }
}


bool check_valid_ticket(Ticket ticket) {
    unsigned int index = hash_id(ticket.id);
    if (hash_table[index].id == -1) {
        return false;
    } else if (hash_table[index].id == ticket.id) {
        return true;
    }
    else {
        int fails = 1;
        while (hash_table[index].id != ticket.id) {
            if (fails >= TICKETCOUNT) return false;
            index += (fails * fails) % TABLESIZE;
            fails++;
        }
        return true;
    }
}


void enter_festival(Ticket ticket) {
    unsigned int index = hash_id(ticket.id);
    if (hash_table[index].id == -1) {
        cout << ">> Can't enter, <" << ticket.id << "> is an invalid ticket." << endl;
        return;
    }
    else if (hash_table[index].id == ticket.id) {
        if (hash_table[index].entered) {
            cout << ">> Can't enter, ticket <" << ticket.id << "> has already entered." << endl;
            return;
        }
        hash_table[index].entered = true;
    }
    else {
        if (hash_table[index].entered) {
            cout << ">> Can't enter, ticket <" << ticket.id << "> has already entered." << endl;
            return;
        }
        int fails = 1;
        while (hash_table[index].id != ticket.id) {
            index += (fails * fails) % TABLESIZE;
            fails++;
        }
        hash_table[index].entered = true;
    }

    entered_tickets.push(ticket);
    cout << ">> Entered successfully with ticket <" << ticket.id << ">." << endl;
}


int main() {
    random_device rand_dev;
    mt19937 rng(rand_dev());

    while (true) {
        int option;
        cout << "1. Sell ticket\n2. Enter with ticket\n3. Check entered\n4. Exit\n\n>?";
        cin >> option;

        if (option == 1) {
            if (entered_tickets.count() > TICKETCOUNT) {
                cout << "Sorry, festival is at capacity." << endl;
            } else {
                uniform_int_distribution<mt19937::result_type> dist(MINTICKET, MAXTICKET);
                Ticket new_ticket = Ticket((int) dist(rng));
                while (check_valid_ticket(new_ticket)) {
                    uniform_int_distribution<mt19937::result_type> dist(MINTICKET, MAXTICKET);
                    Ticket new_ticket = Ticket((int) dist(rng));
                }
                insert_table(hash_id(new_ticket.id), new_ticket);
                cout << ">> Added ticket <" << new_ticket.id << "> to database." << endl;
            }
        } else if (option == 2) {
            int id;
            cout << "Enter ticket number.\n>?";
            cin >> id;

            enter_festival(Ticket(id));
        } else if (option == 3) {
            number_of_people();
        } else if (option == 4) {
            cout << "Exiting ticket management system.";
            break;
        } else {
            cout << "Invalid option selected.";
        }

        cout << endl;
    }
}
