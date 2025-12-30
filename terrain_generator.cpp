#include "terrain_generator.h"

TerrainGenerator::TerrainGenerator(uint8_t initial_seed) {
    seed = initial_seed;
    // Инициализируем высоты для первого экрана
    for (uint8_t i = 0; i < CELL_COUNT_X; i++) {
        last_height[i] = 8; // Средняя высота
    }
}

void TerrainGenerator::setSeed(uint8_t new_seed) {
    seed = new_seed;
    randomSeed(seed);
}

void TerrainGenerator::generateScreen(uint32_t screen[CELL_COUNT_Y]) {
    // Начальная генерация - простая холмистая местность
    int8_t height = 8; // Средняя высота
    
    // Очищаем экран
    for (uint8_t y = 0; y < CELL_COUNT_Y; y++) {
        screen[y] = 0;
    }
    
    // Генерируем начальный склон
    for (uint8_t x = 0; x < CELL_COUNT_X; x++) {
        // Плавное изменение высоты
        int8_t height_change = random(-max_step, max_step + 1);
        height += height_change;
        
        // Ограничиваем высоту
        if (height < min_height) height = min_height;
        if (height > max_height) height = max_height;
        
        last_height[x] = height;
        
        // Заполняем столбец от высоты до низа
        for (uint8_t y = height; y < CELL_COUNT_Y; y++) {
            setCell(screen, x, y, true);
        }
    }
    
    // Сглаживаем
    smoothTerrain(screen);
}

void TerrainGenerator::generateNextScreen(uint32_t new_screen[CELL_COUNT_Y], uint32_t previous_screen[CELL_COUNT_Y]) {
    // Очищаем новый экран
    for (uint8_t y = 0; y < CELL_COUNT_Y; y++) {
        new_screen[y] = 0;
    }
    
    // Определяем высоту в последнем столбце предыдущего экрана
    int8_t last_previous_height = CELL_COUNT_Y - 1;
    for (int8_t y = CELL_COUNT_Y - 1; y >= 0; y--) {
        if (getCell(previous_screen, CELL_COUNT_X - 1, y)) {
            last_previous_height = y;
            break;
        }
    }
    
    // Начинаем с этой высоты
    int8_t current_height = last_previous_height;
    
    // Копируем последний столбец предыдущего экрана как первый нового
    for (uint8_t y = 0; y < CELL_COUNT_Y; y++) {
        if (getCell(previous_screen, CELL_COUNT_X - 1, y)) {
            setCell(new_screen, 0, y, true);
        }
    }
    last_height[0] = current_height;
    
    // Выбираем тип генерации для этого экрана
    uint8_t terrain_type = random(100);
    
    // Генерируем остальные столбцы
    for (uint8_t x = 1; x < CELL_COUNT_X; x++) {
        if (terrain_type < 30) { // 30% - плоская местность
            // Сохраняем высоту
        }
        else if (terrain_type < 60) { // 30% - холмы
            int8_t change = random(-max_step * 2, max_step * 2 + 1);
            current_height += change;
        }
        else if (terrain_type < 80) { // 20% - платформы
            if (x % 8 == 0) { // Каждые 8 клеток возможность платформы
                if (random(100) < 40) {
                    // Создаем платформу
                    uint8_t platform_length = random(2, 6);
                    uint8_t platform_height = random(min_height, current_height);
                    
                    for (uint8_t px = 0; px < platform_length && (x + px) < CELL_COUNT_X; px++) {
                        for (uint8_t y = platform_height; y < CELL_COUNT_Y; y++) {
                            setCell(new_screen, x + px, y, true);
                        }
                        last_height[x + px] = platform_height;
                    }
                    x += platform_length - 1;
                    current_height = platform_height;
                    continue;
                }
            }
            int8_t change = random(-max_step, max_step + 1);
            current_height += change;
        }
        else { // 20% - ступеньки
            if (x % 4 == 0 && random(100) < 50) {
                current_height += (random(2) ? 1 : -1) * 2;
            }
        }
        
        // Ограничиваем высоту
        if (current_height < min_height) current_height = min_height;
        if (current_height > max_height) current_height = max_height;
        
        // Заполняем столбец
        for (uint8_t y = current_height; y < CELL_COUNT_Y; y++) {
            setCell(new_screen, x, y, true);
        }
        
        last_height[x] = current_height;
    }
    
    // Добавляем случайные проходы (пещеры)
    if (random(100) < 30) {
        uint8_t cave_start = random(4, CELL_COUNT_X - 8);
        uint8_t cave_height = random(min_height + 2, max_height - 2);
        uint8_t cave_width = random(4, 10);
        
        for (uint8_t x = cave_start; x < cave_start + cave_width && x < CELL_COUNT_X; x++) {
            // Создаем проход высотой 2-3 клетки
            for (uint8_t y = cave_height; y < cave_height + 3 && y < CELL_COUNT_Y; y++) {
                setCell(new_screen, x, y, false);
            }
        }
    }
    
    // Сглаживаем рельеф
    smoothTerrain(new_screen);
    
    // Обновляем seed для следующей генерации
    seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF;
}

void TerrainGenerator::setCell(uint32_t field[CELL_COUNT_Y], uint8_t x, uint8_t y, bool value) {
    if (x >= CELL_COUNT_X || y >= CELL_COUNT_Y) return;
    
    if (value) {
        field[y] |= (0x80000000 >> x);
    } else {
        field[y] &= ~(0x80000000 >> x);
    }
}

bool TerrainGenerator::getCell(uint32_t field[CELL_COUNT_Y], uint8_t x, uint8_t y) {
    if (x >= CELL_COUNT_X || y >= CELL_COUNT_Y) return false;
    return (field[y] & (0x80000000 >> x)) != 0;
}

void TerrainGenerator::smoothTerrain(uint32_t field[CELL_COUNT_Y]) {
    uint32_t smoothed[CELL_COUNT_Y];
    
    // Копируем поле
    for (uint8_t y = 0; y < CELL_COUNT_Y; y++) {
        smoothed[y] = field[y];
    }
    
    // Сглаживаем - заполняем одиночные отверстия
    for (uint8_t x = 1; x < CELL_COUNT_X - 1; x++) {
        for (uint8_t y = 0; y < CELL_COUNT_Y - 1; y++) {
            // Если клетка пустая, но сверху и снизу есть земля
            if (!getCell(field, x, y) && 
                getCell(field, x, y-1) && 
                getCell(field, x, y+1)) {
                // Заполняем дыру
                setCell(smoothed, x, y, true);
            }
            
            // Если клетка с землей, но сверху и снизу пусто
            if (getCell(field, x, y) && 
                !getCell(field, x, y-1) && 
                !getCell(field, x, y+1) &&
                y > 0 && y < CELL_COUNT_Y - 1) {
                // Убираем одинокий столбик
                setCell(smoothed, x, y, false);
            }
        }
    }
    
    // Копируем обратно
    for (uint8_t y = 0; y < CELL_COUNT_Y; y++) {
        field[y] = smoothed[y];
    }
}