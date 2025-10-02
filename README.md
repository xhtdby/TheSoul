# The Soul
An online seed analyzer for Balatro with advanced game engine for goal-based planning.

## Features

### Seed Analysis
Predict what will happen in your Balatro run:
- Boss blinds for each ante
- Vouchers available in the shop
- Tags received
- Pack types and contents (Jokers, Tarots, Planets, etc.)
- Shop item queue with rarities and editions

### Game Engine (NEW!)
Plan your runs with goal tracking and feasibility checking:
- **Set Goals**: Define targets like "have $150 by ante 4" or "reach 1M score by ante 5"
- **Feasibility Checking**: Analyze if your goals are achievable with the given seed
- **Mid-Game Analysis**: Feed in your current state and analyze the rest of your run
- **Recommendations**: Get strategic advice based on your goals
- **Flexible Queries**: Ask specific questions about any ante

## Usage

### Web Interface
Visit https://spectralpack.github.io/TheSoul/ to use the seed analyzer.

For the new Game Engine features, open `game_engine_example.html` in your browser (requires compiled WebAssembly).

### Command Line (Conceptual)
```bash
python3 game_engine_cli.py --seed ABCD1234 \
    --goal min_money:150:4:mandatory \
    --goal min_score:1000000:5 \
    --simulate 5 \
    --check-feasibility
```

Note: CLI requires Python bindings (future work).

## Documentation

- **[GAME_ENGINE.md](GAME_ENGINE.md)** - Complete Game Engine API documentation with examples
- **[game_engine_example.html](game_engine_example.html)** - Interactive web interface for the Game Engine
- **[game_engine_cli.py](game_engine_cli.py)** - Command-line interface (conceptual)

## Building

The project uses Emscripten to compile C++ to WebAssembly:

```bash
em++ -O3 --closure 1 -lembind -o immolate.js include/immolate.cpp -s EXPORT_NAME="'Immolate'"
```

See `build.bat` for the build script.

## Game Engine Examples

### Example 1: Money Target
```javascript
let engine = new Module.GameEngine("SEED1234");
let state = new Module.GameState();
state.deck = "Yellow Deck";
engine.setState(state);

// Goal: Have $200 by ante 4
engine.addGoal(new Module.GameGoal("min_money", 200, 4, true));

if (engine.checkGoalFeasibility()) {
    console.log("This seed can achieve the goal!");
    console.log(engine.simulateToAnte(4));
}
```

### Example 2: Mid-Game Analysis
```javascript
let engine = new Module.GameEngine("CURRENT1");
let state = new Module.GameState();
state.currentAnte = 3;
state.money = 85;
state.score = 250000;
engine.setState(state);

// Can I reach 1M score by ante 5?
engine.addGoal(new Module.GameGoal("min_score", 1000000, 5, true));
console.log(engine.getFeedback());
```

### Example 3: Strategic Planning
```javascript
let engine = new Module.GameEngine("WINNER99");
engine.addGoal(new Module.GameGoal("min_money", 150, 4, true));

// Get recommendations for ante 2
let recs = engine.getRecommendedActions(2);
for (let i = 0; i < recs.size(); i++) {
    console.log(recs.get(i));
}
```

## Contributing

Contributions are welcome! The codebase includes:
- `include/instance.hpp` - Core seed RNG and state management
- `include/functions.hpp` - Game prediction functions  
- `include/items.hpp` - Game items and data structures
- `include/game_engine.hpp` - NEW: Game engine with goal tracking
- `include/immolate.hpp` - WebAssembly bindings

## License

See repository license.
