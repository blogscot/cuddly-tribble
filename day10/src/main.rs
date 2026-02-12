use itertools::Itertools;

#[derive(Debug, PartialEq, Clone, Eq)]
enum Light {
    On,
    Off,
}

fn parse_numbers(input: &str) -> Vec<u32> {
    input[1..input.len() - 1]
        .split(",")
        .map(|s| s.parse::<u32>().unwrap())
        .collect::<Vec<_>>()
}

#[derive(Debug, PartialEq, Eq)]
struct Lights {
    lights: Vec<Light>,
}

impl Lights {
    fn new(num: usize) -> Self {
        let lights = vec![Light::Off; num];
        Self { lights }
    }

    fn apply(&mut self, buttons: &[u32]) {
        for button in buttons {
            self.lights[*button as usize] = match self.lights[*button as usize] {
                Light::On => Light::Off,
                Light::Off => Light::On,
            }
        }
    }
}

fn parse_input(input: &str) -> (Vec<Light>, Vec<Vec<u32>>, Vec<u32>) {
    let splits: Vec<_> = input.split(" ").collect();
    let lights = &splits[0][1..splits[0].len() - 1]
        .chars()
        .map(|ch| match ch {
            '#' => Light::On,
            '.' => Light::Off,
            _ => unreachable!("Invalid character"),
        })
        .collect::<Vec<_>>();

    let buttons: Vec<_> = splits[1..splits.len() - 1]
        .iter()
        .map(|s| parse_numbers(s))
        .collect();

    let joltage = parse_numbers(splits[splits.len() - 1]);

    (lights.to_vec(), buttons, joltage)
}

fn count_button_presses(input: &str) -> u32 {
    let (answer, buttons, _) = parse_input(input);
    let mut presses = 0;
    buttons.iter().powerset().any(|set| {
        let mut lights = Lights::new(answer.len());
        presses = 0;
        set.iter().for_each(|btns| {
            presses += 1;
            lights.apply(btns);
        });
        lights.lights == answer
    });
    presses
}

fn part1(input: &str) -> u32 {
    input.lines().map(count_button_presses).sum::<u32>()
}

fn main() {
    let data = std::fs::read_to_string("puzzle.txt").unwrap();
    println!("{}", part1(&data));
}
