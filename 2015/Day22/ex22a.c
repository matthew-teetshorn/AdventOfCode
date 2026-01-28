#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE            16
#define MAX_SPELL_NAME      14
#define NO_PLAYER_NUM       0
#define PLAYER_NUM          1
#define BOSS_NUM            2
#define PLAYER_START_HEALTH 50
#define PLAYER_START_MANA   500
#define MAX_SPELLS          8
#define MAX_GAME_STATES     65536

#define SPELL_MM_MANACOST              53
#define SPELL_MM_DMG                   4
#define SPELL_Drain_MANACOST           73
#define SPELL_Drain_DMG                2
#define SPELL_Drain_HEAL               2
#define SPELL_Shield_MANACOST          113
#define SPELL_Shield_Effect_ARMOR      7
#define SPELL_Shield_Effect_DURATION   6
#define SPELL_Poison_MANACOST          173
#define SPELL_Poison_Effect_DMG        3
#define SPELL_Poison_Effect_DURATION   6
#define SPELL_Recharge_MANACOST        229
#define SPELL_Recharge_Effect_MANA     101
#define SPELL_Recharge_Effect_DURATION 5

#define PART_TWO true

typedef enum {
  MAGIC_MISSILE,
  DRAIN,
  SHIELD,
  POISON,
  RECHARGE,
  SPELL_COUNT,
} Spell;

typedef struct {
  int health;
  int manaCurrent;
  int manaSpent;
  int armor;
  bool hasEffectRecharge;
  int turnsRecharge;
  bool hasEffectShielded;
  int turnsShielded;
  int numSpellsCast;
  Spell spellsCast[MAX_SPELLS];
} player_t;

typedef struct {
  int health;
  int damage;
  bool hasEffectPoisened;
  int turnsPoisoned;
} boss_t;

typedef struct {
  player_t player;
  boss_t boss;
  int turn;
} game_state_t;

typedef struct {
  int head;
  int tail;
  int numStates;
  game_state_t states[MAX_GAME_STATES];
} game_buffer_t;

void gameStateInit(game_buffer_t *buffer) {
  buffer->numStates = 0;
  buffer->head      = 0;
  buffer->tail      = 0;
  memset(buffer, 0, MAX_GAME_STATES);
}

bool gameStateInsert(game_buffer_t *buffer, game_state_t *state) {
  if (buffer->numStates >= MAX_GAME_STATES) return false;

  game_state_t *insert = &buffer->states[buffer->head];
  memcpy(insert, state, sizeof(game_state_t));
  buffer->numStates++;
  buffer->head = (buffer->head + 1) % MAX_GAME_STATES;

  return true;
}

bool gameStatePop(game_buffer_t *buffer, game_state_t *outState) {
  if (buffer->numStates == 0) return false;

  memcpy(outState, &buffer->states[buffer->tail], sizeof(game_state_t));
  buffer->numStates--;
  buffer->tail = (buffer->tail + 1) % MAX_GAME_STATES;
  return true;
}

void printGameState(game_state_t *state) {
  printf("\n");
  printf("Game state:\n");
  printf("Boss => health: %d\n", state->boss.health);
  printf("Player => health: %d, manaCurrent: %d, manaSpent: %d\n",
         state->player.health, state->player.manaCurrent,
         state->player.manaSpent);
  for (int i = 0; i < state->player.numSpellsCast; i++) {
    printf("Spell cast: ");
    switch (state->player.spellsCast[i]) {
    case MAGIC_MISSILE:
      printf("Magic Missile\n");
      break;
    case DRAIN:
      printf("Drain\n");
      break;
    case SHIELD:
      printf("Shield\n");
      break;
    case POISON:
      printf("Poison\n");
      break;
    case RECHARGE:
      printf("Recharge\n");
      break;
    case SPELL_COUNT:
      printf("Overran Spell Enum\n");
      break;
    }
  }
  printf("\n");
}

int main() {
  FILE *fp = fopen("boss.input", "r");
  if (fp == NULL) {
    printf("Failed to open boss input\n");
    return EXIT_FAILURE;
  }

  char buffer[MAX_LINE];
  boss_t boss     = {0, 0, 0, 0};
  player_t player = {
      PLAYER_START_HEALTH, PLAYER_START_MANA, 0, 0, 0, 0, 0, 0, 0, {0}};

  // Scan boss stats
  fgets(buffer, MAX_LINE, fp);
  sscanf(buffer, "Hit Points: %d", &boss.health);
  fgets(buffer, MAX_LINE, fp);
  sscanf(buffer, "Damage: %d", &boss.damage);
  fclose(fp);

  // Generate and process fights
  game_buffer_t gameBuffer;
  game_state_t start = {player, boss, PLAYER_NUM};
  gameStateInit(&gameBuffer);
  gameStateInsert(&gameBuffer, &start);

  game_state_t currState;
  game_state_t bestState;
  int min_cost_to_win = INT_MAX;
  while (gameStatePop(&gameBuffer, &currState)) {
    // printGameState(&currState);
    player_t *player = &currState.player;
    boss_t *boss     = &currState.boss;
    int turn         = currState.turn;
    int winner       = NO_PLAYER_NUM;
    bool gameEnded   = false;

    if (PART_TWO && turn == PLAYER_NUM) {
      player->health -= 1;
    }

    if (player->health <= 0) {
      gameEnded = true;
      winner    = BOSS_NUM;
    } else if (boss->health <= 0) {
      gameEnded = true;
      winner    = PLAYER_NUM;
    }

    if (!gameEnded) {
      // Process Effects
      if (player->hasEffectShielded) {
        player->armor = SPELL_Shield_Effect_ARMOR;
        if (--player->turnsShielded <= 0) {
          player->hasEffectShielded  = false;
          player->armor             -= SPELL_Shield_Effect_ARMOR;
          if (player->armor < 0) player->armor = 0;
        }
      }
      if (player->hasEffectRecharge) {
        player->manaCurrent += SPELL_Recharge_Effect_MANA;
        if (--player->turnsRecharge <= 0) {
          player->hasEffectRecharge = false;
        }
      }
      if (boss->hasEffectPoisened) {
        boss->health -= SPELL_Poison_Effect_DMG;
        if (--boss->turnsPoisoned <= 0) {
          boss->hasEffectPoisened = false;
        }
      }
    }

    if (boss->health <= 0) {
      gameEnded = true;
      winner    = PLAYER_NUM;
    } else if (player->numSpellsCast >= MAX_SPELLS && turn == PLAYER_NUM) {
      gameEnded = true;
      winner    = BOSS_NUM;
    }

    if (!gameEnded) {
      player_t next_player;
      boss_t next_boss;

      if (turn == PLAYER_NUM) {
        // Cast spells
        for (int i = 0; i < SPELL_COUNT; i++) {
          memcpy(&next_player, player, sizeof(player_t));
          memcpy(&next_boss, boss, sizeof(boss_t));

          switch (i) {
          case MAGIC_MISSILE:
            if (next_player.manaCurrent >= SPELL_MM_MANACOST) {
              next_player.manaCurrent -= SPELL_MM_MANACOST;
              next_player.manaSpent   += SPELL_MM_MANACOST;
              next_boss.health        -= SPELL_MM_DMG;
              next_player.spellsCast[next_player.numSpellsCast++] =
                  MAGIC_MISSILE;

              if (!gameStateInsert(
                      &gameBuffer,
                      &(game_state_t){next_player, next_boss, BOSS_NUM})) {
                printf("Game State Buffer full: exiting...\n");
                return EXIT_FAILURE;
              }
            }
            break;
          case DRAIN:
            if (next_player.manaCurrent >= SPELL_Drain_MANACOST) {
              next_player.manaCurrent -= SPELL_Drain_MANACOST;
              next_player.manaSpent   += SPELL_Drain_MANACOST;
              next_boss.health        -= SPELL_Drain_DMG;
              next_player.health      += SPELL_Drain_HEAL;
              next_player.spellsCast[next_player.numSpellsCast++] = DRAIN;

              if (!gameStateInsert(
                      &gameBuffer,
                      &(game_state_t){next_player, next_boss, BOSS_NUM})) {
                printf("Game State Buffer full: exiting...\n");
                return EXIT_FAILURE;
              }
            }
            break;
          case SHIELD:
            if (next_player.manaCurrent >= SPELL_Shield_MANACOST &&
                !next_player.hasEffectShielded) {
              next_player.manaCurrent       -= SPELL_Shield_MANACOST;
              next_player.manaSpent         += SPELL_Shield_MANACOST;
              next_player.hasEffectShielded  = true;
              next_player.turnsShielded      = SPELL_Shield_Effect_DURATION;
              next_player.spellsCast[next_player.numSpellsCast++] = SHIELD;

              if (!gameStateInsert(
                      &gameBuffer,
                      &(game_state_t){next_player, next_boss, BOSS_NUM})) {
                printf("Game State Buffer full: exiting...\n");
                return EXIT_FAILURE;
              }
            }
            break;
          case POISON:
            if (next_player.manaCurrent >= SPELL_Poison_MANACOST &&
                !next_boss.hasEffectPoisened) {
              next_player.manaCurrent     -= SPELL_Poison_MANACOST;
              next_player.manaSpent       += SPELL_Poison_MANACOST;
              next_boss.hasEffectPoisened  = true;
              next_boss.turnsPoisoned      = SPELL_Poison_Effect_DURATION;
              next_player.spellsCast[next_player.numSpellsCast++] = POISON;

              if (!gameStateInsert(
                      &gameBuffer,
                      &(game_state_t){next_player, next_boss, BOSS_NUM})) {
                printf("Game State Buffer full: exiting...\n");
                return EXIT_FAILURE;
              }
            }
            break;
          case RECHARGE:
            if (next_player.manaCurrent >= SPELL_Recharge_MANACOST &&
                !next_player.hasEffectRecharge) {
              next_player.manaCurrent       -= SPELL_Recharge_MANACOST;
              next_player.manaSpent         += SPELL_Recharge_MANACOST;
              next_player.hasEffectRecharge  = true;
              next_player.turnsRecharge      = SPELL_Recharge_Effect_DURATION;
              next_player.spellsCast[next_player.numSpellsCast++] = RECHARGE;

              if (!gameStateInsert(
                      &gameBuffer,
                      &(game_state_t){next_player, next_boss, BOSS_NUM})) {
                printf("Game State Buffer full: exiting...\n");
                return EXIT_FAILURE;
              }
            }
            break;
          }
        }
      } else if (turn == BOSS_NUM) {
        memcpy(&next_player, player, sizeof(player_t));
        memcpy(&next_boss, boss, sizeof(boss_t));
        int damage          = next_boss.damage - next_player.armor;
        next_player.health -= damage > 0 ? damage : 1;

        if (!gameStateInsert(
                &gameBuffer,
                &(game_state_t){next_player, next_boss, PLAYER_NUM})) {
          printf("Game State Buffer full: exiting...\n");
          return EXIT_FAILURE;
        }
      }
    }

    if (gameEnded && winner == PLAYER_NUM) {
      if (player->manaSpent <= min_cost_to_win) {
        min_cost_to_win = player->manaSpent;
        bestState       = currState;
      }
    }
  }

  printf("Lowest mana cost to win: %d\n", min_cost_to_win);
  printGameState(&bestState);
}
