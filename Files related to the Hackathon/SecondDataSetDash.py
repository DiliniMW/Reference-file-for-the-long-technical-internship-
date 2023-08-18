# Import packages
from dash import Dash, html, dash_table, dcc, callback, Output, Input
import pandas as pd
import plotly.express as px


df = pd.read_csv('submission2.csv')

app = Dash(__name__)

app.layout = html.Div(
    [
      html.H4("Display of the data of the second dataset"),
        html.P("Select an animation:"),
        dcc.RadioItems(
            id="selection",
            options=["charges/coverage_level -Scatter", "bmi - Bar","smoker - scatter","charges/region-pie"],
            value="charges/coverage_level -Scatter",
        ),
        dcc.Loading(dcc.Graph(id="graph"), type="cube"),
    ]
)


@app.callback(
    Output("graph", "figure"), Input("selection", "value")
)
def display_animated_graph(selection):
    animations = {
              "charges/coverage_level -Scatter": px.scatter(
            df,
            x="coverage_level",
            y="charges",
            animation_frame="age",
            animation_group="region",
            size="children",
            color="medical_history",
            hover_name="region",
            range_y=[0, 150000],
        ),
        "bmi - Bar": px.bar(
            df,
            x="exercise_frequency",
            y="bmi",
            color="medical_history",
            animation_frame="age",
            animation_group="region",
            range_y=[0, 70],
        ),
        "smoker - scatter":px.scatter(
        df,
         x="smoker",
            y="charges",
            animation_frame="age",
            animation_group="gender",
            size="children",
            color="medical_history",
            hover_name="region",
            range_y=[0, 150000],
           

        ),
        "charges/region-pie":px.pie(
        df,
        names="region",
        values="charges",
        color="children",
        )
    }
    return animations[selection]


if __name__ == "__main__":
    app.run_server(debug=True)