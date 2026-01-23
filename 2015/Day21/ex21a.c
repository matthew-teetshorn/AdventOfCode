#include <limits.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#define MAX_LINE      32
#define MAX_ITEM_NAME 11
#define MAX_WEAPONS   5
#define MAX_ARMORS    6
#define MAX_RINGS     22
#define PLAYER_NUM    1
#define BOSS_NUM      2

typedef struct {
  int cost;
  int damage;
  int armor;
  char name[MAX_ITEM_NAME];
} item_t;

typedef struct {
  item_t weapon;
  item_t armor;
  item_t rings;
} loadout_t;

typedef struct {
  int health;
  int damage;
  int armor;
} player_t;

// clang-format off
void printItems(item_t weapons[], int num_weapons,
                item_t armors[],  int num_armors,
                item_t rings[],   int num_rings) {
  printf("%11s %5s %7s %5s\n", "Name", "Cost", "Damage", "Armor");
  printf("===============================\n");
  for (int i = 0; i < num_weapons; i++) {
    item_t *cw = &weapons[i];
    printf("%10s: %5d %7d %5d\n", cw->name, cw->cost, cw->damage, cw->armor);
  }
  printf("\n");

  printf("%11s %5s %7s %5s\n", "Name", "Cost", "Damage", "Armor");
  printf("===============================\n");
  for (int i = 0; i < num_armors; i++) {
    item_t *ca = &armors[i];
    printf("%10s: %5d %7d %5d\n", ca->name, ca->cost, ca->damage, ca->armor);
  }
  printf("\n");

  printf("%11s %5s %7s %5s\n", "Name", "Cost", "Damage", "Armor");
  printf("===============================\n");
  for (int i = 0; i < num_rings; i++) {
    item_t *cr = &rings[i];
    printf("%10s: %5d %7d %5d\n", cr->name, cr->cost, cr->damage, cr->armor);
  }
  printf("\n");
}
// clang-format on

int fight(player_t player, player_t boss) {
  int p_damage =
      player.damage - boss.armor > 0 ? player.damage - boss.armor : 1;
  int b_damage =
      boss.damage - player.armor > 0 ? boss.damage - player.armor : 1;
  int p_moves_to_kill = ceil((float)boss.health / p_damage);
  int b_moves_to_kill = ceil((float)player.health / b_damage);

  if (p_moves_to_kill <= b_moves_to_kill) return PLAYER_NUM;
  return BOSS_NUM;
}

int main() {
  FILE *fp = fopen("boss.input", "r");
  if (fp == NULL) {
    printf("Failed to open boss input\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  player_t boss;
  player_t player = {100, 0, 0};

  // Scan boss stats
  fgets(buffer, MAX_LINE, fp);
  sscanf(buffer, "Hit Points: %d", &boss.health);
  fgets(buffer, MAX_LINE, fp);
  sscanf(buffer, "Damage: %d", &boss.damage);
  fgets(buffer, MAX_LINE, fp);
  sscanf(buffer, "Armor: %d", &boss.armor);
  fclose(fp);

  // weapons.input contains all weapons
  fp = fopen("weapons.input", "r");
  if (fp == NULL) {
    printf("Failed to open weapons input\n");
    return EXIT_FAILURE;
  }

  int num_weapons = 0;
  item_t weapons[MAX_WEAPONS];
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    item_t *wp = &weapons[num_weapons++];
    sscanf(buffer, "%s %d %d %d", wp->name, &wp->cost, &wp->damage, &wp->armor);
  }
  fclose(fp);

  // armors.input contains all armors including "no armor"
  fp = fopen("armors.input", "r");
  if (fp == NULL) {
    printf("Failed to open armors input\n");
    return EXIT_FAILURE;
  }

  int num_armors = 0;
  item_t armors[MAX_ARMORS];
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    item_t *ap = &armors[num_armors++];
    sscanf(buffer, "%s %d %d %d", ap->name, &ap->cost, &ap->damage, &ap->armor);
  }
  fclose(fp);

  // rings.input contains all combinations of 0, 1, or 2 rings
  fp = fopen("rings.input", "r");
  if (fp == NULL) {
    printf("Failed to open rings input\n");
    return EXIT_FAILURE;
  }

  int num_rings = 0;
  item_t rings[MAX_RINGS];
  while (fgets(buffer, MAX_LINE, fp) != NULL) {
    item_t *rp = &rings[num_rings++];
    sscanf(buffer, "%s %d %d %d", rp->name, &rp->cost, &rp->damage, &rp->armor);
  }
  fclose(fp);

  int min_cost_to_win  = INT_MAX;
  int max_cost_to_lose = INT_MIN;
  int num_fights       = 0;
  for (int w = 0; w < num_weapons; w++) {
    for (int a = 0; a < num_armors; a++) {
      for (int r = 0; r < num_rings; r++) {
        num_fights++;
        loadout_t curr_loadout = {weapons[w], armors[a], rings[r]};
        player.damage = curr_loadout.weapon.damage + curr_loadout.rings.damage;
        player.armor  = curr_loadout.armor.armor + curr_loadout.rings.armor;
        int winner    = fight(player, boss);
        // clang-format off
        int cost = curr_loadout.weapon.cost +
                   curr_loadout.armor.cost +
                   curr_loadout.rings.cost;
        // clang-format on
        if (winner == PLAYER_NUM) {
          if (cost < min_cost_to_win) min_cost_to_win = cost;
        } else {
          if (cost > max_cost_to_lose) max_cost_to_lose = cost;
        }
      }
    }
  }

  printf("Over %d fights:\n", num_fights);
  printf("Least cost to win: %d\n", min_cost_to_win);
  printf("Most cost to lose: %d\n", max_cost_to_lose);
}
