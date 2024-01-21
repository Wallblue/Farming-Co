CREATE TABLE IF NOT EXISTS player(
    playerId INTEGER PRIMARY KEY AUTOINCREMENT,
    pseudo VARCHAR(25),
    startDate DATE,
    farmName VARCHAR(25),
    timeInGame INTEGER,
    money INTEGER
);

CREATE TABLE IF NOT EXISTS object(
      objectId INTEGER PRIMARY KEY AUTOINCREMENT,
      x INTEGER,
      y INTEGER,
      w INTEGER,
      h INTEGER,
      zone INTEGER,
      growTime INTEGER,
      poseDate INTEGER,
      state INTEGER,
      boosted BOOLEAN,
      itemId INTEGER,
      playerId INTEGER,
      FOREIGN KEY (itemId) REFERENCES item(itemId),
      FOREIGN KEY (playerId) REFERENCES player(playerId),
      UNIQUE(x, y, zone)
);

CREATE TABLE IF NOT EXISTS npc(
    npcId INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(25),
    sprite VARCHAR(128),
    trader BOOLEAN,
    x INTEGER,
    y INTEGER,
    zone INTEGER
);

CREATE TABLE IF NOT EXISTS item(
    itemId INTEGER PRIMARY KEY AUTOINCREMENT,
    name VARCHAR(25),
    type VARCHAR(25),
    description TEXT,
    energyBonus INTEGER,
    ability INTEGER,
    sprite VARCHAR(128),
    growTime INTEGER,
    linkedObjectSpriteRef CHARACTER(1),
    evolution INTEGER,
    linkedTool INTEGER,
    sellingPrice INTEGER,
    FOREIGN KEY (evolution) REFERENCES item(itemId),
    FOREIGN KEY (linkedTool) REFERENCES item(itemId)
);

CREATE TABLE IF NOT EXISTS dialog(
      dialogId INTEGER PRIMARY KEY AUTOINCREMENT,
      content TEXT,
      npcId INTEGER,
      FOREIGN KEY (npcId) REFERENCES npc(npcId)
);

CREATE TABLE IF NOT EXISTS PLAYER_OWN(
        itemId INTEGER,
        playerId INTEGER,
        quantity INTEGER DEFAULT 0,
        slot INTEGER,
        FOREIGN KEY (playerId) REFERENCES player(playerId),
        FOREIGN KEY (itemId) REFERENCES item(itemId),
        CONSTRAINT pk PRIMARY KEY (itemId, playerId, slot)
);

CREATE TABLE IF NOT EXISTS NPC_OWN(
        itemId INTEGER,
        npcId INTEGER,
        buyingPrice INTEGER,
        quantity INTEGER DEFAULT 0,
        sold INTEGER DEFAULT 0,
        FOREIGN KEY (npcId) REFERENCES npc(npcId),
        FOREIGN KEY (itemId) REFERENCES item(itemId),
        CONSTRAINT pk PRIMARY KEY (itemId, npcId)
);

CREATE TABLE IF NOT EXISTS OBJECT_OWN(
        itemId INTEGER,
        objectId INTEGER,
        quantity INTEGER DEFAULT 0,
        slot INTEGER,
        FOREIGN KEY (itemId) REFERENCES item(itemId),
        FOREIGN KEY (objectId) REFERENCES npc(npcId),
        CONSTRAINT pk PRIMARY KEY (itemId, objectId, slot)
);
