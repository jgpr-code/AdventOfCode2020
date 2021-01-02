#include <algorithm>
#include <cassert>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

using ll = long long;

// try out designated initializers (c++20)

struct Range {
  ll low;
  ll high;
};

struct FieldRanges {
  Range low_range;
  Range high_range;
};

struct TicketField {
  string name;
  FieldRanges ranges;
};

struct TicketSpecification {
  vector<TicketField> allowed_fields;
};

struct Ticket {
  vector<ll> fields;
};

TicketField parse_ticket_field(const string& line) {
  string capture_name = "(.*)";
  string capture_range = "([[:digit:]]+)-([[:digit:]]+)";
  regex ticket_field_regex(capture_name + ": " + capture_range + " or " +
                           capture_range);
  smatch matches;
  regex_match(line, matches, ticket_field_regex);
  return {.name = matches[1].str(),
          .ranges = {.low_range = {.low = stoll(matches[2].str()),
                                   .high = stoll(matches[3].str())},
                     .high_range = {.low = stoll(matches[4].str()),
                                    .high = stoll(matches[5].str())}}};
}

Ticket parse_ticket(const string& line) {
  vector<ll> fields;
  istringstream iss(line);
  for (string num; getline(iss, num, ',');) {
    fields.push_back(stoll(num));
  }
  return {.fields = move(fields)};
}

bool isInRange(ll num, const Range& range) {
  return num >= range.low && num <= range.high;
}

bool isInFieldRanges(ll num, const FieldRanges& ranges) {
  return isInRange(num, ranges.low_range) || isInRange(num, ranges.high_range);
}

bool isValidForField(ll num, const TicketField& field) {
  return isInFieldRanges(num, field.ranges);
}

bool isValidForOneOrMoreFields(ll num,
                               const TicketSpecification& specification) {
  for (const auto field : specification.allowed_fields) {
    if (isValidForField(num, field)) return true;
  }
  return false;
}

ll sumInvalidFieldsForTicket(const Ticket& ticket,
                             const TicketSpecification& specification) {
  ll sum = 0;
  for (const auto field : ticket.fields) {
    if (!isValidForOneOrMoreFields(field, specification)) sum += field;
  }
  return sum;
}

int main() {
  TicketSpecification ticket_specification;
  vector<Ticket> nearby_tickets;
  vector<Ticket> valid_tickets;
  Ticket my_ticket;
  string line;

  for (; getline(cin, line);) {
    if (line == "") break;
    ticket_specification.allowed_fields.push_back(parse_ticket_field(line));
  }

  getline(cin, line);
  assert(line == "your ticket:");
  getline(cin, line);
  my_ticket = parse_ticket(line);
  valid_tickets.push_back(my_ticket);

  getline(cin, line);
  assert(line == "");

  getline(cin, line);
  assert(line == "nearby tickets:");
  for (; getline(cin, line);) {
    nearby_tickets.push_back(parse_ticket(line));
  }

  ll sum_of_invalid = 0;
  for (const auto ticket : nearby_tickets) {
    ll s = sumInvalidFieldsForTicket(ticket, ticket_specification);
    if (s == 0) {
      valid_tickets.push_back(ticket);
    } else {
      sum_of_invalid += s;
    }
  }
  cout << "Part 1: " << sum_of_invalid << endl;
}
