#[derive(Debug, Clone, Copy, Default)]
struct Tile {
    x: u64,
    y: u64,
}

impl Tile {
    fn new(x: u64, y: u64) -> Tile {
        Tile { x, y }
    }
    fn area(&self, other: &Tile) -> u64 {
        (self.x.abs_diff(other.x) + 1) * (self.y.abs_diff(other.y) + 1)
    }
}

#[derive(Debug, PartialEq, Clone, Copy)]
enum LineType {
    Horizontal,
    Vertical,
    Point,
}

#[derive(Debug, Clone)]
struct Line {
    a: Tile,
    b: Tile,
    line_type: LineType,
}

impl Line {
    fn new(t1: Tile, t2: Tile) -> Line {
        let line_type = match (t1.x == t2.x, t1.y == t2.y) {
            (true, false) => LineType::Vertical,
            (false, true) => LineType::Horizontal,
            (true, true) => LineType::Point,
            _ => panic!("Invalid line"),
        };

        let (a, b) = if (line_type == LineType::Vertical && t1.y > t2.y)
            || (line_type == LineType::Horizontal && t1.x > t2.x)
        {
            (t2, t1)
        } else {
            (t1, t2)
        };
        Line { a, b, line_type }
    }
}

fn _part1(mut tiles: Vec<Tile>) -> u64 {
    let mut max_area = 0;
    while let Some(tile) = tiles.pop() {
        tiles.iter().for_each(|t| {
            let diff = tile.area(t);
            if diff > max_area {
                max_area = diff;
            }
        })
    }
    max_area
}

// A tile is valid if it has bounding lines in all 4 directions, i.e. North, East, South, West
fn is_tile_valid(lines: &[Line], tile: &Tile) -> bool {
    // Bounding lines
    let vertical_iter = lines
        .iter()
        .filter(|line| {
            line.line_type == LineType::Vertical && line.a.y <= tile.y && line.b.y >= tile.y
        })
        .collect::<Vec<_>>();
    let horizontal_iter = lines
        .iter()
        .filter(|line| {
            line.line_type == LineType::Horizontal && line.a.x <= tile.x && line.b.x >= tile.x
        })
        .collect::<Vec<_>>();
    vertical_iter.iter().any(|line| tile.x <= line.a.x) // east
        && vertical_iter.iter().any(|line| line.a.x <= tile.x) // west
        && horizontal_iter.iter().any(|line| tile.y <= line.a.y) // north
        && horizontal_iter.iter().any(|line| line.a.y <= tile.y) // south
}

fn check_line(lines: &[Line], line: &Line) -> bool {
    match line.line_type {
        LineType::Horizontal => {
            (line.a.x..=line.b.x).all(|x| is_tile_valid(&lines, &Tile::new(x, line.a.y)))
        }
        LineType::Vertical => {
            (line.a.y..=line.b.y).all(|y| is_tile_valid(&lines, &Tile::new(line.a.x, y)))
        }
        LineType::Point => true,
    }
}

fn part2(tiles: Vec<Tile>) -> u64 {
    let lines = (0..tiles.len() - 1)
        .flat_map(|i| {
            (i + 1..tiles.len())
                .filter_map(|j| {
                    if tiles[i].x == tiles[j].x || tiles[i].y == tiles[j].y {
                        Some(Line::new(tiles[i], tiles[j]))
                    } else {
                        None
                    }
                })
                .collect::<Vec<_>>()
        })
        .collect::<Vec<Line>>();

    // (0..9).for_each(|y| {

    //     (0..14).for_each(|x| {
    //         if is_tile_valid(&lines, &Tile::new(x as u64, y as u64)) {
    //             print!("O");
    //         } else {
    //             print!(".");
    //         }
    //     });
    //     print!("\n");
    // })

    // Pair each tile with each other tile, which would respresent a possible area.
    // Determine each line of the area
    // Filter out possible areas that do not have valid lines.
    // Find the max area
    (0..tiles.len() - 1)
        .flat_map(|i| {
            let tiles = tiles.clone();
            let lines = lines.clone();
            (i + 1..tiles.len())
                .filter_map(move |j| {
                    let corner1 = tiles[i];
                    // Note, there are occasions where the width or height of the area is 1.
                    // This means trying to create a line using the same point, which is currently
                    // invalid.
                    let corner2 = Tile::new(tiles[j].x, tiles[i].y);
                    let corner3 = tiles[j];
                    let corner4 = Tile::new(tiles[i].x, tiles[j].y);
                    // println!("{:?} {:?} {:?} {:?}", corner1, corner2, corner3, corner4);
                    let line1 = Line::new(corner1, corner2);
                    let line2 = Line::new(corner2, corner3);
                    let line3 = Line::new(corner3, corner4);
                    let line4 = Line::new(corner4, corner1);
                    if [line1, line2, line3, line4]
                        .iter()
                        .all(|line| check_line(&lines, line))
                    {
                        Some((tiles[i], tiles[j]))
                    } else {
                        None
                    }
                })
                .map(|(tile1, tile2)| tile1.area(&tile2))
                .collect::<Vec<_>>()
        })
        // .collect::<Vec<u64>>()
        .max()
        .unwrap()
}

fn main() {
    let data = std::fs::read_to_string("puzzle.txt").unwrap();
    let tiles = data
        .lines()
        .map(|line| line.split(",").collect::<Vec<&str>>())
        .map(|v| Tile::new(v[0].parse::<u64>().unwrap(), v[1].parse::<u64>().unwrap()))
        .collect::<Vec<_>>();
    // tiles.iter().for_each(|tile| println!("{:?}", tile));

    // println!("{:?}", part1(tiles));
    println!("{:?}", part2(tiles));
}

// 4623431865
